// MicroNix Wi-Fi Sync Clock by Alex Riensche
//
// This program operates the MicroNix Nixie Tube Clock
// Displays the current time from an grtc and updates
// the clock via Wi-Fi once per day

// DEV NOTES
// I think the issue with time manager is related to the references for the class, any suggestion on fixes are appreciated.

#include "main.h"
#include "uTimerLib.h"

#include "wifiDefines.h"
#include "Credentials.h"
#include "dynamicParams.h"

// CLASS INSTANSIATION
MCP7940_Class grtc;
CRGB leds[numLED];
U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI display(U8G2_R0, /* CS=*/ CS,/* reset=*/ DC, /* reset=*/ U8X8_PIN_NONE);
Encoder enc(encoderPinA, encoderPinB);

WiFiUDP gudp;                            // udp instance  
IPAddress gtimeServer(129, 6, 15, 28);   // time server class
DateTime gnow(0);


// REFERENCE POINTERS

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

// Time Sync Library Pointer
timeManager time;   

// Nixie Control Library Pointer
nixieManager* nixie;


// Wifi Variables
WiFiManager_NINA_Lite* wifiManager;

void setup() {
  // begin debug serial
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

  // begin the udp instance for wifi packets
  bool state = gudp.begin(localPort);

  // check on status of udp connection
  Serial.print("udp state ");
  Serial.println(state);

  delay(1000);

  Serial.println("start pins");
  // set up pin modes
  // INPUT
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);

  // OUTPUT
  pinMode(tubePin_a, OUTPUT);
  pinMode(tubePin_b, OUTPUT);
  pinMode(tubePin_c, OUTPUT);
  pinMode(tubePin_d, OUTPUT);
  pinMode(periodPin, OUTPUT);

  pinMode(anodePin_a, OUTPUT);
  pinMode(anodePin_b, OUTPUT);
  pinMode(anodePin_c, OUTPUT);
  pinMode(anodePin_d, OUTPUT);


  Serial.println("start RTC");
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

  Serial.println("start time");

  gnow = 0;
  



  nixieManager snixie(gleftHour, grightHour, gleftMin, grightMin, ghourZero, gminZero, gperiods, gcathodeTime, gnow);
  nixie = &snixie;

  Serial.println("Start RGB");
  // initialize RGB Leds
  FastLED.addLeds<WS2812, ledPin, GRB>(leds, numLED);

  Serial.println("Start OLED");
  // start OLED screen
  display.begin();

  Serial.println("start wifi");
  // start wifi manager
  wifiManager = new WiFiManager_NINA_Lite();
  wifiManager->begin();

  Serial.println("start timer interrupt");
  // establish timer interrupt
  TimerLib.setInterval_us(nixieISR, 4000);  

  Serial.println("startup complete");
}


// LOOP
void loop() {
  static unsigned long timeRefresh = millis();
  //static DateTime testTime = DateTime(now);
  
  
  wifiManager->run();
//  Serial.println("run");
  DateTime testTime = grtc.now(); // test line to active the RTC get command, this works
//  Serial.println(testTime.second());
  
  // update the clock and time periodically
  long timePassed = millis() - timeRefresh;
  if(abs(timePassed > 2000))
  {
    Serial.println(timePassed > 2000);
    time.get();
//    Serial.println("passed time update");
    printStatus();
//    nixie->setTime();

    timeRefresh = millis();
  }

  
  delay(20);
}




// GLOBAL HELPER FUNCTIONS
void nixieISR()
{
  static byte tubeIndex = 0;
  if (gnixieActive)
  {
    switch (tubeIndex)
    {     
      case 0:
        clearNixie(3); // shut off tube 4
        if(ghourZero || gleftHour != 0)
        {
          setNixie(0, gleftHour);
        }
        tubeIndex++;
        break;
      case 1:
        clearNixie(0);
        setNixie(1, grightHour);
        tubeIndex++;
        break;
      case 2:
        clearNixie(1);
        if(gminZero || gleftHour != 0)
        {
          setNixie(2, gleftMin);
        }
        tubeIndex++;
        break;
      case 3:
        clearNixie(2);
        setNixie(3, grightMin);
        tubeIndex++;
        break;
      default:
        tubeIndex = 0;
    }
    setNixiePeriod(tubeIndex, gperiods[tubeIndex]);
  }
  
}

// SN74141 : Truth Table
//D C B A #
//L,L,L,L 0
//L,L,L,H 1
//L,L,H,L 2
//L,L,H,H 3
//L,H,L,L 4
//L,H,L,H 5
//L,H,H,L 6
//L,H,H,H 7
//H,L,L,L 8
//H,L,L,H 9

void setNixie(byte tubeSelect, byte value)
{
  byte a,b,c,d;
  a = 0; b = 0; c = 0; d = 0;
  switch( value )
  {
    case 0: a=0;b=0;c=0;d=0;break;
    case 1: a=1;b=0;c=0;d=0;break;
    case 2: a=0;b=1;c=0;d=0;break;
    case 3: a=1;b=1;c=0;d=0;break;
    case 4: a=0;b=0;c=1;d=0;break;
    case 5: a=1;b=0;c=1;d=0;break;
    case 6: a=0;b=1;c=1;d=0;break;
    case 7: a=1;b=1;c=1;d=0;break;
    case 8: a=0;b=0;c=0;d=1;break;
    case 9: a=1;b=0;c=0;d=1;break;
    default: a=1;b=1;c=1;d=1;break;
  }
  
   // Write the number binary
   digitalWrite(tubePin_a, a);
   digitalWrite(tubePin_b, b);
   digitalWrite(tubePin_c, c);
   digitalWrite(tubePin_d, d);

   // Activate the correct anode
   if(value > 9)
   {
    digitalWrite(anodes[tubeSelect], LOW);
   }
   else
   {
    digitalWrite(anodes[tubeSelect], HIGH);
   }
   
}

void setNixiePeriod(byte tubeSelect, bool state)
{
  digitalWrite(periodPin, state);
  digitalWrite(anodes[tubeSelect], HIGH);
}

void clearNixie(byte tubeSelect)
{
  digitalWrite(tubePin_a, LOW);
  digitalWrite(tubePin_b, LOW);
  digitalWrite(tubePin_c, LOW);
  digitalWrite(tubePin_d, LOW);
  digitalWrite(periodPin, LOW);
  digitalWrite(anodes[tubeSelect], LOW);
}

void printStatus ()
{
  Serial.print(gnow.hour());
  Serial.print(":");
  Serial.print(gnow.minute());
  Serial.print(":");
  Serial.println(gnow.second());
  Serial.println(gnow.unixtime());
}
