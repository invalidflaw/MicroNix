#include "nixieManager.h"


/* ******************************************************************** */
nixieManager::nixieManager()
/* ******************************************************************** */
{
//  lastCathodeTime = 0;
}


/* ******************************************************************** */
void nixieManager::setTime()
/* ******************************************************************** */
{
  Serial.println("read time numbers"); 
  byte hour = gnow.hour();
  byte minute = gnow.minute();

  // update the nixie values
  Serial.println("update tube values");
  gleftHour = hour / 10 % 10;
  grightHour = hour % 10;
  
  gleftMin = minute / 10 % 10;
  grightMin = minute % 10;

  Serial.println("cathode check");
  if(gnow.unixtime() > lastCathodeTime + (gcathodeTime * 60))
  {
    cathodeProtect();
    Serial.println("run cathode protect");
    lastCathodeTime = gnow.unixtime();
  }

  
}


/* ******************************************************************** */
void nixieManager::cathodeProtect()
/* ******************************************************************** */
{
  // lock out user interface, run protect algorithm
  // enable the left hour in minute temporarily if needed
  bool hourOff = false;
  bool minOff = false;

  if (!ghourZero)
  {
    hourOff = true;
    ghourZero = true;
  }
  if (!gminZero)
  {
    minOff = true;
    gminZero = true;
  }
  for (int i = 0; i < 51; i++)
  {
    Serial.println("cathode protect");
    if(i < 21)
    {
      grightMin++;
      if(grightMin > 9) grightMin = 0;
    }

    if(i < 31)
    {
      gleftMin++;
      if(gleftMin > 9) gleftMin = 0;
    }

    if( i < 41)
    {
      grightHour++;
      if(grightHour > 9) grightHour = 0;
    }
    
    gleftHour++;   
    if(gleftHour > 9) gleftHour = 0;

    
    delay(100);
  }
  if (hourOff)
  {
    ghourZero = false;
  }
  if (minOff)
  {
    gminZero = false;
  }
}
