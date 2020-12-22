#pragma once

#include "config.h"

// HARDWARE DEFINES
#include "Arduino.h"
#include <Wire.h>               // I2C Arduino Library
#include <SPI.h>                // SPI Arduino Library
#include <WiFiNINA_Generic.h>   // Wifi Operating Library
#include <Encoder.h>            // Encoder Counting Library
#include <MCP7940.h>            // external RTC library
#include <FastLED.h>            // neopixel LED library
#include <U8g2lib.h>            // LCD library
#include <LCDMenuLib2.h>        // LCD Menu Library
#include "uTimerLib.h"          // timer interrupt library
#include <ArduinoJson.h>        // json library(weather data)


// MODULE DEFINES
#include "timeManager.h"
#include "nixieManager.h"
#include "ledManager.h"
#include "lcdMenu.h"
#include "nixieInterrupt.h"
