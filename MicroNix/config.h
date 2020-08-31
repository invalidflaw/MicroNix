#pragma once

#define encoderPinA 9
#define encoderPinB 10
#define buttonPin 14

#define tubePin_a 2
#define tubePin_b 3
#define tubePin_c 4
#define tubePin_d 5
#define periodPin 15

#define anodePin_a 7
#define anodePin_b 8
#define anodePin_c 20
#define anodePin_d 21
const int anodes[4] = {anodePin_a, anodePin_b, anodePin_c, anodePin_d};

#define CS 17
#define DC 16

#define ledPin 6    // Neopixel Driver Pin
#define numLED 4
#define ledBrightness 64
#define LED_TYPE WS2812
#define colorOrder GRB
#define ledRefresh 20

#define rtcAddr 0x27

#define SECRET_SSID "The LAN Before Time"
#define SECRET_PASS "itsonthefridge"

#define NTP_PACKET_SIZE 48
#define localPort 2390
