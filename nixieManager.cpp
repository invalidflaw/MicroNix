#include "nixieManager.h"

nixieManager::nixieManager(byte& ileftHour, byte& irightHour, byte& ileftMin, byte& irightMin,bool& ihourZero, bool& iminZero, bool (&iperiods)[4], int& icathodeTime,DateTime& inow):leftHour(ileftHour),
                           rightHour(irightHour), leftMin(ileftMin), rightMin(irightMin), hourZero(ihourZero), minZero(iminZero), periods(iperiods), cathodeTime(icathodeTime), now(inow)
{
  lastCathodeTime = now.unixtime();
}

void nixieManager::setTime()
{
  // 
  byte hour = now.hour();
  byte minute = now.minute();

  // update the nixie values
  leftHour = hour / 10 % 10;
  rightHour = hour % 10;
  
  leftMin = minute / 10 % 10;
  rightMin = minute % 10;

  if(now.unixtime() > lastCathodeTime + (cathodeTime * 60))
  {
    cathodeProtect();
    Serial.println("run cathode protect");
    lastCathodeTime = now.unixtime();
  }

  
}

void nixieManager::cathodeProtect()
{
  // lock out user interface, run protect algorithm
  // enable the left hour in minute temporarily if needed
  bool hourOff = false;
  bool minOff = false;

  if (!hourZero)
  {
    hourOff = true;
    hourZero = true;
  }
  if (!minZero)
  {
    minOff = true;
    minZero = true;
  }
  for (int i = 0; i < 51; i++)
  {
    if(i < 21)
    {
      rightMin++;
      if(rightMin > 9) rightMin = 0;
    }

    if(i < 31)
    {
      leftMin++;
      if(leftMin > 9) leftMin = 0;
    }

    if( i < 41)
    {
      rightHour++;
      if(rightHour > 9) rightHour = 0;
    }
    
    leftHour++;   
    if(leftHour > 9) leftHour = 0;

    
    delay(100);
  }
  if (hourOff)
  {
    hourZero = false;
  }
  if (minOff)
  {
    minZero = false;
  }
}
