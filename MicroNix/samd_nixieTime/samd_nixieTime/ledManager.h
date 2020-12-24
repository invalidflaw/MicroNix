#pragma once
#include "main.h"

// extern data declarations
extern CRGB leds[numLED];
extern bool gholidayFun;

enum class ledColor{
  red,
  blue,
  green,
  yellow,
  aqua,
  purple,
  white,
  rainbow,
  randomDay,
  weather
};

enum class patterns{
  solid,
  pulse,
  flow
};
//
//// pattern functions
//void showSolidColor();
//void pulseSolidColor();
//void flowSolidColor();
//
//// helper lighting functions
//CRGB getColor();

class ledManager
{
  public:
    ledManager();

    void update();
    void setSpeed(byte speed);
    void setBright(byte bright);
    void setPattern(patterns pattern);
    void setColor(ledColor color);

  private:
    void showSolidColor();
    void pulseSolidColor();
    void flowSolidColor();

    CRGB getColor(byte stringLED);

  private:
    patterns selPattern = patterns::pulse;
    ledColor selColor = ledColor::red;
    byte patternSpeed = 3;
    byte ledBright = 15;
};
