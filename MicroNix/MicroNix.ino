// MicroNix Wi-Fi Sync Clock by Alex Riensche
//
// This program operates the MicroNix Nixie Tube Clock
// Displays the current time from an grtc and updates
// the clock via Wi-Fi once per day

// DEV NOTES
// I think the issue with time manager is related to the references for the class, any suggestion on fixes are appreciated.

#include "main.h"
#include "images.h"
#include "uTimerLib.h"

#include "wifiDefines.h"
#include "Credentials.h"
#include "dynamicParams.h"


DynamicJsonDocument jsonBuffer(1000);


// CLASS INSTANSIATION
MCP7940_Class grtc;
CRGB leds[numLED];
U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI display(U8G2_R0, /* CS=*/ CS,/* reset=*/ DC, /* reset=*/ U8X8_PIN_NONE);
Encoder enc(encoderPinA, encoderPinB);

WiFiUDP gudp;                            // udp instance  
IPAddress gtimeServer(129, 6, 15, 28);   // time server class
DateTime gnow(0);

// Create Flash Storage Instances
//FlashStorage(my_flash_store, int);
FlashStorage(utcOffset, int);
FlashStorage(autoDST, bool);
FlashStorage(milTime, bool);
FlashStorage(protectTime, int);
FlashStorage(autoShutoff, bool);
FlashStorage(startTime, int);
FlashStorage(stopTime, int);
FlashStorage(showZero, bool);

void menuDisplay();

LCDMenuLib2_menu LCDML_0 (255, 0, 0, NULL, NULL); // root menu element
LCDMenuLib2 LCDML(LCDML_0, _LCDML_DISP_rows, _LCDML_DISP_cols,menuDisplay, menuClear, menuControl);

// Menu Construction
// Dev Notes, addadvanced calls may be formatted incorrectly
LCDML_add         (0  , LCDML_0      , 1  , "Clock Settings"     , NULL);
LCDML_addAdvanced (1  , LCDML_0_1    , 1  , NULL   , ""          , offsetParam   ,  0  ,  _LCDML_TYPE_dynParam);  // dynamic int parameter
LCDML_addAdvanced (2  , LCDML_0_1    , 2  , NULL   , ""          , dstParam      ,  0  ,  _LCDML_TYPE_dynParam);  // boolean parameter
LCDML_addAdvanced (3  , LCDML_0_1    , 3  , NULL   , ""          , milParam      ,  0  ,  _LCDML_TYPE_dynParam);  // enum boolean parameter   //change parameter callback
LCDML_addAdvanced (4  , LCDML_0_1    , 4  , NULL   , ""          , dynCathParam  ,  0  ,  _LCDML_TYPE_dynParam);  // dynamic int parameter
LCDML_add         (5  , LCDML_0_1    , 5  , "Shutoff Settings"   , NULL);  
LCDML_addAdvanced (6  , LCDML_0_1_5  , 1  , NULL   , ""          , shutoffParam  ,  0  ,  _LCDML_TYPE_dynParam);  // boolean parameter
LCDML_addAdvanced (7  , LCDML_0_1_5  , 2  , NULL   , ""          , startParam    ,  0  ,  _LCDML_TYPE_dynParam);  // int parameter, 30 min interval
LCDML_addAdvanced (8  , LCDML_0_1_5  , 3  , NULL   , ""          , stopParam     ,  0  ,  _LCDML_TYPE_dynParam);  // int parameter, 30 min interval
LCDML_add         (9  , LCDML_0_1_5  , 4  , "Save Parameters"    , saveParamCall);
LCDML_add         (10 , LCDML_0_1_5  , 5  , "Back"               , menuBack);
LCDML_addAdvanced (11 , LCDML_0_1    , 6  , NULL   , ""          , zeroParam     ,  0  ,  _LCDML_TYPE_dynParam);  // boolean parameter
LCDML_add         (12 , LCDML_0_1    , 7  , "Save Parameters"    , saveParamCall);
LCDML_add         (13 , LCDML_0_1    , 8  , "Back"               , NULL);
LCDML_add         (14 , LCDML_0      , 2  , "LED Settings"       , NULL);
LCDML_add         (15 , LCDML_0_2    , 1  , "LED Color"          , NULL);  // enum parameter
LCDML_add         (16 , LCDML_0_2    , 2  , "LED Effect"         , NULL);  // enum parameter
LCDML_add         (17 , LCDML_0_2    , 3  , "LED Brightness"     , NULL);  // enum parameter
LCDML_add         (18 , LCDML_0_2    , 4  , "Save Parameters"    , saveParamCall);
LCDML_add         (19 , LCDML_0_2    , 5  , "Back"               , menuBack);
LCDML_add         (20 , LCDML_0      , 3  , "Other Settings"     , NULL);
LCDML_add         (21 , LCDML_0      , 4  , "Save Parameters"    , saveParamCall);
LCDML_add         (22 , LCDML_0      , 5  , "Reset Wifi"         , NULL);
LCDML_add         (23 , LCDML_0      , 6  , "Exit"               , screenSaver);



// menu element count - last element id
// this value must be the same as the last menu element
#define _LCDML_DISP_cnt    23

// create menu
LCDML_createMenu(_LCDML_DISP_cnt);

// TIMER VARIABLES
byte gleftHour = 10;
byte grightHour = 10;
byte gleftMin = 10;
byte grightMin = 10;
int  gtimeZone = -4;
int  gcathodeTime = 15;
bool gdstMode = true;
bool gperiods[4] = {false,false,false,false};
bool ghourZero = false;
bool gminZero = true;   
bool gnixieActive = false;
bool gtimeCalibrated = true;

// Time Control Library
timeManager time;   

// Nixie Tube Manager
nixieManager nixie;

// Wifi Variables
WiFiManager_NINA_Lite* wifiManager;


/* ******************************************************************** */
void setup() 
/* ******************************************************************** */
  {
  /*
   * HARDWARE SETUP ORDER
   * 1. Serial(debug purposes)
   * 2. UDP
   * 3. PINS
   * 3. RTC, grtc class setup
   * 4. RGB LEDS
   * 5  FLASH STORAGE (proposed init to dynamParams class
   * 6. DISPLAY
   * 7. LCD MENU
   * 8. WIFI MANAGER
   * 9. TIMER INTERRUPT
   */
    
  // begin debug serial
  Serial.begin(9600);

  // begin the udp instance for wifi packets
  bool state = gudp.begin(localPort);

  // check on status of udp connection
  Serial.print("udp state ");
  Serial.println(state);

  delay(1000);
  
  // INPUT PINS
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);

  // OUTPUT PINS
  pinMode(tubePin_a, OUTPUT);
  pinMode(tubePin_b, OUTPUT);
  pinMode(tubePin_c, OUTPUT);
  pinMode(tubePin_d, OUTPUT);
  pinMode(periodPin, OUTPUT);

  pinMode(anodePin_a, OUTPUT);
  pinMode(anodePin_b, OUTPUT);
  pinMode(anodePin_c, OUTPUT);
  pinMode(anodePin_d, OUTPUT);

  // grtc Configuration
  while (!grtc.begin()) { // Initialize grtc communications
    Serial.println("Unable to find MCP7940. Checking again in 1 second.");
    delay(500);
  } // of loop until device is located

  if(!grtc.deviceStatus())
  {
    grtc.deviceStart();
    gtimeCalibrated = false;
  }

  if(!grtc.getBattery())
  {
    grtc.setBattery(true);    
  }

  if(grtc.getPowerFail())
  {
    grtc.clearPowerFail();
  }

  gnow = 0;

  // initialize RGB Leds
  FastLED.addLeds<WS2812, ledPin, GRB>(leds, numLED).setCorrection(TypicalLEDStrip);

  // intitialize settings from flash storage values
  loadParams();

  // start display
  Serial.println("Start OLED");
  display.begin();
  display.clearBuffer();
  display.drawXBM(0,0,huskerX, huskerY, huskerN);
  display.sendBuffer();
  delay(1000);

  // start lcd menu
  LCDML_setup(_LCDML_DISP_cnt);
  LCDML.MENU_enRollover();
  LCDML.SCREEN_enable(screenSaver, 10000); // set to 10 seconds


  Serial.println("start wifi");
  // start wifi manager
  wifiManager = new WiFiManager_NINA_Lite();
  wifiManager->begin();

  // establish timer interrupt
  TimerLib.setInterval_us(nixieISR, 4000);  

  Serial.println("startup complete");
  display.clear();
}


/* ******************************************************************** */
// MAIN LOOP
void loop() 
/* ******************************************************************** */
{
  static unsigned long timeRefresh = millis();
  //static DateTime testTime = DateTime(now);
  Serial.println("loop");
  wifiManager->run();
  LCDML.loop();
  
  // update the clock and time periodically
  long timePassed = millis() - timeRefresh;
  if(abs(timePassed > 2000))
  {
    time.get();
    Serial.println("passed time update");
    printStatus();
//    nixie.setTime();    // this is causing a program hang

    timeRefresh = millis();
  }

  
  delay(20);
}




// GLOBAL HELPER FUNCTIONS

/* ******************************************************************** */
void printStatus ()
/* ******************************************************************** */
{
  Serial.print(gnow.hour());
  Serial.print(":");
  Serial.print(gnow.minute());
  Serial.print(":");
  Serial.println(gnow.second());
  Serial.println(gnow.unixtime());
}


/* ******************************************************************** */
void menuDisplay()
/* ******************************************************************** */
{
  // for first test set font here
  display.setFont(_LCDML_DISP_font);

  // declaration of some variables
  // ***************
  // content variable
  char content_text[_LCDML_DISP_cols];  // save the content text of every menu element
  // menu element object
  LCDMenuLib2_menu *tmp;
  // some limit values
  uint8_t i = LCDML.MENU_getScroll();
  uint8_t maxi = _LCDML_DISP_rows + i;
  uint8_t n = 0;

   // init vars
  uint8_t n_max             = (LCDML.MENU_getChilds() >= _LCDML_DISP_rows) ? _LCDML_DISP_rows : (LCDML.MENU_getChilds());

  uint8_t scrollbar_min     = 0;
  uint8_t scrollbar_max     = LCDML.MENU_getChilds();
  uint8_t scrollbar_cur_pos = LCDML.MENU_getCursorPosAbs();
  uint8_t scroll_pos        = ((1.*n_max * _LCDML_DISP_rows) / (scrollbar_max - 1) * scrollbar_cur_pos);

  // generate content
  display.clearBuffer();
  n = 0;
  i = LCDML.MENU_getScroll();
  // update content
  // ***************

  // clear menu
  // ***************

  // check if this element has children
  if ((tmp = LCDML.MENU_getDisplayedObj()) != NULL)
  {
    // loop to display lines
    do
    {
      // check if a menu element has a condition and if the condition be true
      if (tmp->checkCondition())
      {
        // check the type off a menu element
        if(tmp->checkType_menu() == true)
        {
          // display normal content
          LCDML_getContent(content_text, tmp->getID());
          display.drawStr( _LCDML_DISP_box_x0+_LCDML_DISP_font_w + _LCDML_DISP_cur_space_behind, _LCDML_DISP_box_y0 + _LCDML_DISP_font_h * (n + 1), content_text);
        }
        else
        {
          if(tmp->checkType_dynParam()) {
            tmp->callback(n);
          }
        }
        // increment some values
        i++;
        n++;
      }
    // try to go to the next sibling and check the number of displayed rows
    } while (((tmp = tmp->getSibling(1)) != NULL) && (i < maxi));
  }

  // set cursor
  display.drawStr( _LCDML_DISP_box_x0+_LCDML_DISP_cur_space_before, _LCDML_DISP_box_y0 + _LCDML_DISP_font_h * (LCDML.MENU_getCursorPos() + 1),  _LCDML_DISP_cursor_char);

  if(_LCDML_DISP_draw_frame == 1) {
     display.drawFrame(_LCDML_DISP_box_x0, _LCDML_DISP_box_y0, (_LCDML_DISP_box_x1-_LCDML_DISP_box_x0), (_LCDML_DISP_box_y1-_LCDML_DISP_box_y0));
  }

  // display scrollbar when more content as rows available and with > 2
  if (scrollbar_max > n_max && _LCDML_DISP_scrollbar_w > 2)
  {
    // set frame for scrollbar
    display.drawFrame(_LCDML_DISP_box_x1 - _LCDML_DISP_scrollbar_w, _LCDML_DISP_box_y0, _LCDML_DISP_scrollbar_w, _LCDML_DISP_box_y1-_LCDML_DISP_box_y0);

    // calculate scrollbar length
    uint8_t scrollbar_block_length = scrollbar_max - n_max;
    scrollbar_block_length = (_LCDML_DISP_box_y1-_LCDML_DISP_box_y0) / (scrollbar_block_length + _LCDML_DISP_rows);

    //set scrollbar
    if (scrollbar_cur_pos == 0) {                                   // top position     (min)
      display.drawBox(_LCDML_DISP_box_x1 - (_LCDML_DISP_scrollbar_w-1), _LCDML_DISP_box_y0 + 1                                                     , (_LCDML_DISP_scrollbar_w-2)  , scrollbar_block_length);
    }
    else if (scrollbar_cur_pos == (scrollbar_max-1)) {            // bottom position  (max)
      display.drawBox(_LCDML_DISP_box_x1 - (_LCDML_DISP_scrollbar_w-1), _LCDML_DISP_box_y1 - scrollbar_block_length                                , (_LCDML_DISP_scrollbar_w-2)  , scrollbar_block_length);
    }
    else {                                                                // between top and bottom
      display.drawBox(_LCDML_DISP_box_x1 - (_LCDML_DISP_scrollbar_w-1), _LCDML_DISP_box_y0 + (scrollbar_block_length * scrollbar_cur_pos + 1),(_LCDML_DISP_scrollbar_w-2)  , scrollbar_block_length);
    }
  }
  display.sendBuffer();
}


/* ******************************************************************** */
void saveParams()
/* ******************************************************************** */
{
  utcOffset.write(dynamParams::utcOffset);
  autoDST.write(dynamParams::autoDST);
  milTime.write(dynamParams::milTime);
  protectTime.write(dynamParams::protectTime);
  autoShutoff.write(dynamParams::autoShutoff);
  startTime.write(dynamParams::startTime);
  stopTime.write(dynamParams::stopTime);
  showZero.write(dynamParams::showZero);
}

/* ******************************************************************** */
void loadParams()
/* ******************************************************************** */
{
  dynamParams::utcOffset = utcOffset.read();
  dynamParams::autoDST = autoDST.read();
  dynamParams::milTime = milTime.read();
  dynamParams::protectTime = protectTime.read();
  dynamParams::autoShutoff = autoShutoff.read();
  dynamParams::startTime = startTime.read();
  dynamParams::stopTime = stopTime.read();
  dynamParams::showZero = showZero.read();
}
