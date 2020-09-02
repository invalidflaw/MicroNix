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
