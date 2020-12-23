#include "espData.h"

String getWeather()
{
  SerialNina.print("getWeather>");
  while(!SerialNina.available())
  {
    delay(10);
  }
  String reply = SerialNina.readStringUntil('>');
  return reply;
}

unsigned long getTime()
{
  SerialNina.print("gettime>");
  while(!SerialNina.available())
  {
    delay(10);
  }
  String reply = SerialNina.readStringUntil('>');
  unsigned long timeData = reply.toInt();
  Serial.println(timeData);
  return timeData;
}

void configureNetwork()
{
  SerialNina.print("portal>");
}
