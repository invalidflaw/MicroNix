#include <SPIFFS.h>
#include <WiFiSettings.h>
#include <HTTPClient.h>

const String apiKey = "&APPID=1f217b9e242a7ee3867adb9b4bae55b2";
const String apiCall = "http://api.openweathermap.org/data/2.5/weather?q=";

String locationData;
String weatherData;
int checkTime = 60000;

// TEST strings
char txMsg[] = "<stringtest>\n";
char rxMsg[20] = "";

TaskHandle_t weatherTask;

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
  
  xTaskCreatePinnedToCore(
    weatherGet,
    "weather Get",
    2000,
    NULL,
    1,
    &weatherTask,
    0
  );    
}

void loop() 
{
  //check for a serial message from the SAMD21
  if(Serial.available())
  {
    String message = Serial.readStringUntil('>');
    if(message == "getweather")
    {
      Serial.print(weatherData + ">");
    }
    else if(message == "portal")
    {
      WiFiSettings.portal();
    }
  }
  delay(100);
}

void weatherGet(void * parameter)
{
  static unsigned long lastCheck = 90000000000;
  while(true)
  {
    unsigned long curTime = millis();
    if ((WiFi.status() == WL_CONNECTED) && abs(curTime - lastCheck) > checkTime)
    {
//      Serial.println(apiCall + locationData + apiKey);
      HTTPClient http;
      http.begin(apiCall + locationData + apiKey);
      int httpCode = http.GET();  //Make the request
      if (httpCode > 0) { //Check for the returning code
        weatherData = http.getString();
      }
      http.end();
      lastCheck = curTime;
    }
    delay(1000);    
  }
}
