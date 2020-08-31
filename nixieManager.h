#pragma once
#include "main.h"

class nixieManager
{
  public:
  nixieManager(byte& ileftHour, byte& irightHour, byte& ileftMin, byte& irightMin,bool& ihourZero, bool& iminZero,
               bool (&iperiods)[4],int& icathodeTime ,DateTime& inow);

    void cathodeProtect();

    void setTime();

    void setPeriod();

    void setBlink();

    void update();

  private:
    byte& leftHour;
    byte& rightHour;
    byte& leftMin;
    byte& rightMin;
    bool& hourZero;
    bool& minZero;
    bool (&periods)[4];
    int&  cathodeTime;
    DateTime& now;

    unsigned long lastCathodeTime;
};
