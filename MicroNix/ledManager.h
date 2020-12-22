#pragma once
#include "main.h"

// extern data declarations
extern CRGB leds[numLED];

enum class ledColor{
  red,
  blue,
  green,
  yellow,
  aqua,
  purple,
  rainbow,
  white
};

enum class ledPalette{
  rainbow,
  forest,
  lava
};

enum class ledEffect{
  solid,
  solidGlitter,
  pusleGlitter,
  pulse,
  juggle,
  confetti
};

class ledManager
{
  public:
    ledManager();

    void update();
    void setColor(ledColor color);
    void setPalette(ledPalette color);
    void setEffect(ledEffect effect);
    void setBright(byte ledBright);
    void setEffectRate(byte ledEffectRate);

  private:
    

  private:
    byte color = 0;
    byte palette = 0;
    byte effect = 0;
    byte effectRate = 1;
    byte brightness = 50;
  
};
