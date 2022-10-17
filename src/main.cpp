#include <WiFiMulti.h>
#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <WiFi.h>

#include <cstdio>
using namespace std;

//#include <../include/wifi.cpp>
#include <../lib/ocs_arduino_library/src/opencommunitysafety.cpp>
#include <../lib/ocs_arduino_library/src/WebServer.cpp>
#include "AsyncJson.h"

using namespace websockets;
using namespace ocs;

WebAdmin ocsWebAdmin(80);

Preferences pref;
String deviceId = "";

ocs::OpenCommunitySafety ocsClass;
const int gpio_in_01 = 32;
const int gpio_out_01 = 21;
const String websockets_server_host = "wss://open-community-safety.herokuapp.com/ws/device";

const char *echo_org_ssl_ca_cert = R"(-----BEGIN CERTIFICATE-----
MIIEDzCCAvegAwIBAgIBADANBgkqhkiG9w0BAQUFADBoMQswCQYDVQQGEwJVUzEl
MCMGA1UEChMcU3RhcmZpZWxkIFRlY2hub2xvZ2llcywgSW5jLjEyMDAGA1UECxMp
U3RhcmZpZWxkIENsYXNzIDIgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMDQw
NjI5MTczOTE2WhcNMzQwNjI5MTczOTE2WjBoMQswCQYDVQQGEwJVUzElMCMGA1UE
ChMcU3RhcmZpZWxkIFRlY2hub2xvZ2llcywgSW5jLjEyMDAGA1UECxMpU3RhcmZp
ZWxkIENsYXNzIDIgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwggEgMA0GCSqGSIb3
DQEBAQUAA4IBDQAwggEIAoIBAQC3Msj+6XGmBIWtDBFk385N78gDGIc/oav7PKaf
8MOh2tTYbitTkPskpD6E8J7oX+zlJ0T1KKY/e97gKvDIr1MvnsoFAZMej2YcOadN
+lq2cwQlZut3f+dZxkqZJRRU6ybH838Z1TBwj6+wRir/resp7defqgSHo9T5iaU0
X9tDkYI22WY8sbi5gv2cOj4QyDvvBmVmepsZGD3/cVE8MC5fvj13c7JdBmzDI1aa
K4UmkhynArPkPw2vCHmCuDY96pzTNbO8acr1zJ3o/WSNF4Azbl5KXZnJHoe0nRrA
1W4TNSNe35tfPe/W93bC6j67eA0cQmdrBNj41tpvi/JEoAGrAgEDo4HFMIHCMB0G
A1UdDgQWBBS/X7fRzt0fhvRbVazc1xDCDqmI5zCBkgYDVR0jBIGKMIGHgBS/X7fR
zt0fhvRbVazc1xDCDqmI56FspGowaDELMAkGA1UEBhMCVVMxJTAjBgNVBAoTHFN0
YXJmaWVsZCBUZWNobm9sb2dpZXMsIEluYy4xMjAwBgNVBAsTKVN0YXJmaWVsZCBD
bGFzcyAyIENlcnRpZmljYXRpb24gQXV0aG9yaXR5ggEAMAwGA1UdEwQFMAMBAf8w
DQYJKoZIhvcNAQEFBQADggEBAAWdP4id0ckaVaGsafPzWdqbAYcaT1epoXkJKtv3
L7IezMdeatiDh6GX70k1PncGQVhiv45YuApnP+yz3SFmH8lU+nLMPUxA2IGvd56D
eruix/U0F47ZEUD0/CwqTRV/p2JdLiXTAAsgGh1o+Re49L2L7ShZ3U0WixeDyLJl
xy16paq8U4Zt3VekyvggQQto8PT7dL5WXXp59fkdheMtlb71cZBDzI0fmgAKhynp
VSJYACPq4xJDKVtHCN2MQWplBqjlIapBtJUhlbl90TSrE9atvNziPTnNvT51cKEY
WQPJIrSPnNVeKtelttQKbfi3QBFGmh95DmK/D5fs4C8fF5Q=
-----END CERTIFICATE-----
)";

WiFiMulti wifiMulti;

void cb(WiFiClient *client, String path, String method)
{
    Serial.println("Entra en el callback " + method);
}

void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}

void getSettings(AsyncWebServerRequest *request)
{
    String outputJson = "";
    serializeJson(ocsClass.toJson(), outputJson);
    request->send(200, "application/json", outputJson);
}

AsyncCallbackJsonWebHandler *handlerBody = new AsyncCallbackJsonWebHandler("/setsettings", [](AsyncWebServerRequest *request, JsonVariant &json)
                                                                           {
ocsClass.setFromJson(json);
    request->send(200, "application/json", "{}"); });

void setup()
{

    Serial.begin(115200);
    delay(10);

    ocsWebAdmin.on("/getsettings", getSettings);
    // ocsWebAdmin.on("/setsettings", HTTP_POST, onRequest, onUpload, setSettings);
    ocsWebAdmin.addHandler(handlerBody); // Para poder leer el body enviado en el request

    ocsWebAdmin.onNotFound(notFound);
    ocsWebAdmin.setup();
    //   ocsWebAdmin.onRequest(cb);
    ocsClass.setSSID(0, "ocsdefault", "ocs@qwerty"); // Valor por default
    ocsClass.setSSID(1, "edwinspire", "Caracol1980");
    ocsClass.setSSID(2, "edwinspiremovil", "libp1983");

    ocsClass.setup(websockets_server_host, deviceId, gpio_in_01, gpio_out_01, echo_org_ssl_ca_cert);

    delay(1000);

    int size = ocsClass.getMaxSSIDs();

    for (uint i = 0; i < size; i = i + 1)
    {
        WifiParams wps = ocsClass.getSSID(i);
        if (wps.ssid.length() > 5)
        {
            Serial.println("Add SSID => " + wps.ssid);
            wifiMulti.addAP(wps.ssid.c_str(), wps.pwd.c_str());
        }
    }

    Serial.println("Connecting Wifi...");

    if (wifiMulti.run() == WL_CONNECTED)
    {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        ocsWebAdmin.begin();
        // server.begin();
        // ocsWebAdmin.setup();

        ocsClass.connectWS();
    }
}

void loop()
{

    if (wifiMulti.run() != WL_CONNECTED)
    {
        Serial.println("WiFi not connected!");
        delay(1000);
    }

    ocsClass.loop();
}
