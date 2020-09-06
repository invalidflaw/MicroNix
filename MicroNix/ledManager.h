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
  

  private:
};
