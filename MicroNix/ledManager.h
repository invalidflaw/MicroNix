#pragma once
#include "main.h"

enum class ledColor{
  red,
  blue,
  green,
  yellow,
  aqua,
  purple,
  cycle,
  rainbow,
  juggle,
  white
};

enum class ledPallete{
  defaultpallete,
  custom
};

enum class ledEffect{
  glitter,
  pulse,
  bpm,
  confetti
};

class ledManager
{
  public:
  ledManager();

  void update();
  void setMode(ledColor color);
  

  private:
};
