#include "ledManager.h"

ledManager::ledManager()
{
  
}

//  public:
//    ledManager();
//
//    void update();
//    void setSpeed(byte speed);
//    void setBright(byte bright);
//    void setPattern(patterns pattern);
//    void setColor(ledColor color);
//
//  private:
//    void showSolidColor();
//    void pulseSolidColor();
//    void flowSolidColor();
//
//    CRGB getColor();

void ledManager::update()
{
  switch(selPattern)
  {
    case patterns::solid:;
      showSolidColor();
      break;
    case patterns::pulse:
      pulseSolidColor();
      break;
    case patterns::flow:
      flowSolidColor(); // TODO: add this method, its currently blank
      break;
  }
  
  FastLED.setBrightness(ledBright);
  FastLED.show();
}

void ledManager::setSpeed(byte speed)
{
  patternSpeed = speed;
}

void ledManager::setBright(byte bright)
{
  ledBright = bright;
}

void ledManager::setPattern(patterns pattern)
{
  selPattern = pattern;
}

void ledManager::setColor(ledColor color)
{
  selColor = color;
}


// HELPER FUNCTIONS
void ledManager::showSolidColor()
{
  for (int i = 0; i < 4; i++)
  {
    CRGB colorResult = getColor(i);
    leds[i] = colorResult;
  }
}

void ledManager::pulseSolidColor()
{
  static int fadeBrightness = 0;
  static int multiplier = 1;
  for (int i = 0;i < 4; i++)
  {
    CRGB colorResult = getColor(i);
    leds[i] = colorResult;
    leds[i].fadeLightBy(fadeBrightness);
  }
  fadeBrightness += (patternSpeed * multiplier);
  if(fadeBrightness < 0 || fadeBrightness > 245)
  {
    multiplier *= -1;
    if(fadeBrightness < 0)
    {
      fadeBrightness = 0;
    }
    else
    {
      fadeBrightness = 245;
    }
  }
}

void ledManager::flowSolidColor()
{
  // TODO: add
}

CRGB ledManager::getColor(byte stringLED)
{
  static byte gHue = 0;
  CRGB colorResult;
  //selects color for each LED. This function has been made to allow for more complex behaviors later on
  switch(selColor)
  {
    case ledColor::red:
      colorResult = CRGB::Red;
      break;
    case ledColor::blue:
      colorResult = CRGB::Blue;
      break;
    case ledColor::green:
      colorResult = CRGB::Green;
      break;
    case ledColor::yellow:
      colorResult = CRGB::Yellow;
      break;
    case ledColor::aqua:
      colorResult = CRGB::Aqua;
      break;
    case ledColor::purple:
      colorResult = CRGB::Purple;
      break;
    case ledColor::white:
      colorResult = CRGB::Gray;
      break;
    case ledColor::rainbow:
      colorResult = CHSV(gHue, 255,255);
      break;
    case ledColor::randomDay:
      //should check if the day has changed, if so pick a random color to display
      colorResult = CRGB::Black; // TODO: add
    case ledColor::weather:
      //should check the temperature and match it to a color pallete value
      colorResult = CRGB::Black; // TODO: add
  }
  // index ghue periodically
  EVERY_N_MILLISECONDS(100)
  {
    gHue++;
  }
  
  return colorResult;
}
