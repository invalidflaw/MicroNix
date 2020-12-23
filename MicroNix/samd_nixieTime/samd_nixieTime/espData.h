#pragma once
#include "main.h"

// Get Weather from openweatherAPI from NINA ESP32
String getWeather();

// Get Epoch NTP Time from NINA ESP32
unsigned long getTime();

// Send command to open wifi settings portal
void configureNetwork();
