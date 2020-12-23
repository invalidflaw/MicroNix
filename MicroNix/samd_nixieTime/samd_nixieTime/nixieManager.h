#pragma once
#include "main.h"

extern byte gleftHour;
extern byte grightHour;
extern byte gleftMin;
extern byte grightMin;
extern int  gcathodeTime;
extern bool gperiods[4];
extern bool ghourZero;
extern bool gminZero;   
extern bool gnixieActive;
extern bool gtimeCalibrated;
extern DateTime gnow;

class nixieManager
{
  public:
  nixieManager();

    void cathodeProtect();

    void setTime();

    void setPeriod();

    void setBlink();

    void update();

  private:
    unsigned long lastCathodeTime = 0;
};
