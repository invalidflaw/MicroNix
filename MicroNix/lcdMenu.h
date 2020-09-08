#pragma once
#include "main.h"

extern LCDMenuLib2 LCDML;
extern Encoder enc;
extern U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI display;

// lcd menu display function
//void menuDisplay();

// lcd menu clear function
void menuClear();

// lcd menu user input function
void menuControl();

void screenSaver(uint8_t param);

void menuBack(uint8_t param);

void offsetParam(uint8_t param);


// class for volatile storage of flash stored parameters for LCD settings
class dynamParams
{
  public:
    dynamParams();

  public:
    inline static int utcOffset = 0;
    inline static bool autoDST = true;
    inline static bool milTime = false;
    inline static int protectTime = 15;
    inline static bool autoShutoff = false;
    inline static int startTime = 7;
    inline static int stopTime = 18;
    inline static bool showZero = false;
  
};
