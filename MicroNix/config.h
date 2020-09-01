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


// LCD MENU OPTIONS


// settings for u8g lib and LCD
#define _LCDML_DISP_w                 128            // LCD width
#define _LCDML_DISP_h                 64             // LCD height
// font settings
#define _LCDML_DISP_font              u8g_font_6x13  // u8glib font (more fonts under u8g.h line 1520 ...)
#define _LCDML_DISP_font_w            6              // font width
#define _LCDML_DISP_font_h            13             // font height
// cursor settings
#define _LCDML_DISP_cursor_char       "X"            // cursor char
#define _LCDML_DISP_cur_space_before  2              // cursor space between
#define _LCDML_DISP_cur_space_behind  4              // cursor space between
// menu position and size
#define _LCDML_DISP_box_x0            0              // start point (x0, y0)
#define _LCDML_DISP_box_y0            0              // start point (x0, y0)
#define _LCDML_DISP_box_x1            128            // width x  (x0 + width)
#define _LCDML_DISP_box_y1            64             // hight y  (y0 + height)
#define _LCDML_DISP_draw_frame        1              // draw a box around the menu

// scrollbar width
#define _LCDML_DISP_scrollbar_w       6  // scrollbar width (if this value is < 3, the scrollbar is disabled)
// nothing change here
#define _LCDML_DISP_cols_max          ((_LCDML_DISP_box_x1-_LCDML_DISP_box_x0)/_LCDML_DISP_font_w)
#define _LCDML_DISP_rows_max          ((_LCDML_DISP_box_y1-_LCDML_DISP_box_y0-((_LCDML_DISP_box_y1-_LCDML_DISP_box_y0)/_LCDML_DISP_font_h))/_LCDML_DISP_font_h)

// rows and cols
// when you use more rows or cols as allowed change in LCDMenuLib.h the define "_LCDML_DISP_cfg_max_rows" and "_LCDML_DISP_cfg_max_string_length"
// the program needs more ram with this changes
#define _LCDML_DISP_rows              _LCDML_DISP_rows_max  // max rows
#define _LCDML_DISP_cols              20                   // max cols
