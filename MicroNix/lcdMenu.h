#pragma once
#include "main.h"
#include "ledManager.h"

extern LCDMenuLib2 LCDML;
extern Encoder enc;
extern U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI display;

// external flash storage objects
extern void saveParams();

//extern ledColor();

// lcd menu display function
void menuDisplay();

// lcd menu clear function
void menuClear();

// lcd menu user input function
void menuControl();

// lcd save parameters callback
void saveParamCall(uint8_t param);

// lcd dynamic parameter functions
void screenSaver(uint8_t param);
void menuBack(uint8_t param);
void offsetParam(uint8_t line);
void dstParam(uint8_t line);
void milParam(uint8_t line);
void dynCathParam(uint8_t line);
void shutoffParam(uint8_t line);
void startParam(uint8_t line);
void stopParam(uint8_t line);
void zeroParam(uint8_t line);

// dynamic helper funcions
void dynEnterCheck();
void dynBoolParameter(bool* param);
void dynIntParameter(int* param, int increment = 1);

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
    inline static int lampColor = 0;

    
  
};
