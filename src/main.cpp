#include <WiFiMulti.h>
#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>
#include <Preferences.h>
//#include <WiFi.h>

//#include <../include/wifi.cpp>
#include <../lib/ocs_arduino_library/src/opencommunitysafety.cpp>

using namespace websockets;
using namespace ocs;

//WiFiServer server(8080);
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
// Variable to store the HTTP request
String header;

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

void setup()
{
    // Before connecting, set the ssl fingerprint of the server
    // client.setCACert(echo_org_ssl_ca_cert);
    /*
        pref.begin("deviceid", true);
        deviceId = pref.getString("deviceid", "00a0aa00-aa00-0000-0000-000000000000");
        pref.end();
    */

    Serial.begin(115200);
    delay(10);

    ocsClass.setSSID("edwinspire", "Caracol1980");
    ocsClass.setSSID("edwinspiremovil", "libpq1983");

    ocsClass.setup(websockets_server_host, deviceId, gpio_in_01, gpio_out_01, echo_org_ssl_ca_cert);

    delay(1000);

    int size = sizeof(ocsClass.listSSID) / sizeof(ocsClass.listSSID[0]);

    for (byte i = 0; i < size; i = i + 1)
    {
        if (ocsClass.listSSID[i].ssid.length() > 5)
        {
            Serial.println("Add SSID => " + ocsClass.listSSID[i].ssid);
            wifiMulti.addAP(ocsClass.listSSID[i].ssid.c_str(), ocsClass.listSSID[i].pwd.c_str());
        }
    }

    Serial.println("Connecting Wifi...");
/*
    int try_connect = 100;
    while (wifiMulti.run() != WL_CONNECTED)
    {
        sleep(2000);
        if (try_connect < 1)
        {
            break;
        }
        try_connect--;
         Serial.println("Connecting Wifi......");
    }
    */

    if (wifiMulti.run() == WL_CONNECTED)
    {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());

       // server.begin();

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

  //  WiFiClient client = server.available(); // Listen for incoming clients
/*
    if (client && false)
    { // If a new client connects,
        currentTime = millis();
        previousTime = currentTime;
        Serial.println("New Client."); // print a message out in the serial port
        String currentLine = "";       // make a String to hold incoming data from the client
        while (client.connected() && currentTime - previousTime <= timeoutTime)
        { // loop while the client's connected
            currentTime = millis();
            if (client.available())
            {                           // if there's bytes to read from the client,
                char c = client.read(); // read a byte, then
                Serial.write(c);        // print it out the serial monitor
                header += c;
                if (c == '\n')
                { // if the byte is a newline character
                    // if the current line is blank, you got two newline characters in a row.
                    // that's the end of the client HTTP request, so send a response:
                    if (currentLine.length() == 0)
                    {
                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                        // and a content-type so the client knows what's coming, then a blank line:
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println("Connection: close");
                        client.println();


                        // Display the HTML web page
                        client.println("<!DOCTYPE html><html>");
                        client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                        client.println("<link rel=\"icon\" href=\"data:,\">");
                        // CSS to style the on/off buttons
                        // Feel free to change the background-color and font-size attributes to fit your preferences
                        client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
                        client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
                        client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
                        client.println(".button2 {background-color: #555555;}</style></head>");

                        // Web Page Heading
                        client.println("<body><h1>ESP32 Web Server</h1>");

                        // Display current state, and ON/OFF buttons for GPIO 26
                        client.println("<p>GPIO 26 - State  output26State </p>");
                        // If the output26State is off, it displays the ON button
                        client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");

                        // Display current state, and ON/OFF buttons for GPIO 27
                        client.println("<p>GPIO 27 - State lllllll</p>");
                        // If the output27State is off, it displays the ON button
                        client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");

                        client.println("</body></html>");

                        // The HTTP response ends with another blank line
                        client.println();
                        // Break out of the while loop
                        break;
                    }
                    else
                    { // if you got a newline, then clear currentLine
                        currentLine = "";
                    }
                }
                else if (c != '\r')
                {                     // if you got anything else but a carriage return character,
                    currentLine += c; // add it to the end of the currentLine
                }
            }
        }
        // Clear the header variable
        header = "";
        // Close the connection
        client.stop();
        Serial.println("Client disconnected.");
        Serial.println("");
    }
*/
    ocsClass.loop();
    // WifiLoop();
}
