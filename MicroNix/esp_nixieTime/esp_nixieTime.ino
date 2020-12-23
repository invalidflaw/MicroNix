#include <SPIFFS.h>
#include <WiFiSettings.h>
#include <HTTPClient.h>
#include <NTPClient_Generic.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

#if !( defined(ESP8266) ||  defined(ESP32) )
  #error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.
#endif

//#define TIME_ZONE_OFFSET_HRS            (-6)

const String apiKey = "&APPID=1f217b9e242a7ee3867adb9b4bae55b2";
const String apiCall = "http://api.openweathermap.org/data/2.5/weather?q=";

String locationData;  // openweathermap location string
String weatherData;   // openweathermap response string
int checkTime = 60000;// time between weather checks,(unused at current)

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

DynamicJsonDocument doc(1024);

void setup() {
  Serial.begin(115200);
  SPIFFS.begin(true);
  delay(100);
  Serial.println("startup");

  // Use stored credentials to connect to your WiFi access point.
  // If no credentials are stored or if the access point is out of reach,
  // an access point will be started with a captive portal to configure WiFi.
  locationData = WiFiSettings.string("time_location","{city name},{state code}");
  checkTime = 60000 * WiFiSettings.integer("check interval(min)", 10);// String name, [long min, long max,] int init = 0, String label = name
  WiFiSettings.connect();

  checkWeatherOnline();
  deserializeJson(doc, weatherData);
  int timeZone = doc["timezone"];
  
  timeClient.begin();
  timeClient.setTimeOffset(timeZone);
  timeClient.setUpdateInterval(SECS_IN_HR);
     
}

void loop() 
{ 
  //check for a serial message from the SAMD21
  if(Serial.available())
  {

    String message = Serial.readStringUntil('>');
    if(message == "getweather")
    {
      if(WiFi.status() == WL_CONNECTED)
      {
        checkWeatherOnline();
        Serial.print(weatherData + '>'); 
      }
      
    }
    else if(message == "gettime")
    {
      timeClient.update();
      if(timeClient.updated())
      {
        Serial.print(timeClient.getEpochTime() + '>');
      }
    }
    else if(message == "portal")
    {
      WiFiSettings.portal();
    }
  }
  delay(100);
}

void checkWeatherOnline()
{
  if(WiFi.status() == WL_CONNECTED)
      {
        HTTPClient http;
        http.begin(apiCall + locationData + apiKey);
        int httpCode = http.GET();  //Make the request
        if (httpCode > 0) //Check for the returning code
        { 
          weatherData = http.getString();
        }
        http.end();
      }
}
