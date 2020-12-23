#pragma once
#include "main.h"

extern byte gleftHour;
extern byte grightHour;
extern byte gleftMin;
extern byte grightMin;
extern int  gtimeZone;
extern int  gcathodeTime;
extern bool gdstMode;
extern bool gperiods[4];
extern bool ghourZero;
extern bool gminZero;   
extern bool gnixieActive;
extern bool gtimeCalibrated;

void nixieISR();
void setNixie(byte tubeSelect, byte value);
void setNixiePeriod(byte tubeSelect, bool state);
void clearNixie(byte tubeSelect);
