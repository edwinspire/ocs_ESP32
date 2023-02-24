#ifdef ESP32
#include <WiFi.h>
#include <WiFiMulti.h>
#elif defined(ESP8266)
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFi.h>
#endif

const String ESP_BOARD = "ESP32_DEVKITC_V4";

#include <Interval.cpp>
#include <opencommunitysafety.cpp>

using namespace ocs;

#ifdef ESP32
WiFiMulti wifiMulti;
#elif defined(ESP8266)
ESP8266WiFiMulti wifiMulti;
#endif

ocs::OpenCommunitySafety ocsClass;
edwinspire::Interval intervalConnectWiFi;

#ifdef ESP32

const int gpio_in_01 = 32;
const int gpio_out_01 = 21;

#elif defined(ESP8266)

const int gpio_in_01 = 0;
const int gpio_out_01 = 2;

#endif

void wifi_reconnect()
{
  Serial.println(F("wifi_reconnect..."));
  Serial.println(WiFi.status());

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Está conectado...");
  }
  else
  {
    Serial.println(F("Connecting Wifi..."));
    if (wifiMulti.run() == WL_CONNECTED)
    {
      Serial.println(F("WiFi connected"));
      Serial.println(F("IP address: "));
      Serial.println(WiFi.localIP());
      Serial.println(WiFi.SSID());
      ocsClass.ip = WiFi.localIP().toString();
      ocsClass.ssid = WiFi.SSID();
      ocsClass.begin();
      ocsClass.connectWS();
    }
    // WiFi.disconnect();
  }
}

void setup()
{

  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(5000);
  ocsClass.setup();

  Serial.println("MAX_SSID_WIFI => " + String(ocs::MAX_SSID_WIFI));
  for (byte i = 0; i < ocs::MAX_SSID_WIFI; i = i + 1)
  {
    Serial.println("SSID => " + ocsClass.ConfigParameter.wifi[i].ssid);
    if (ocsClass.ConfigParameter.wifi[i].ssid.length() > 5)
    {
      Serial.println("Add SSID => " + ocsClass.ConfigParameter.wifi[i].ssid);
      wifiMulti.addAP(ocsClass.ConfigParameter.wifi[i].ssid.c_str(), ocsClass.ConfigParameter.wifi[i].pwd.c_str());
    }
  }

  wifi_reconnect();
  intervalConnectWiFi.setup(15000, &wifi_reconnect); // check wifi each 15 seconds

}

void loop()
{
  ocsClass.loop();
  intervalConnectWiFi.loop();
}