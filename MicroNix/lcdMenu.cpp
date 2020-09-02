#include "lcdMenu.h"
#include <LCDMenuLib2.h>
// global variables for lcd menu system
unsigned long  g_LCDML_CONTROL_button_press_time = millis();
bool  g_LCDML_CONTROL_button_prev       = HIGH;

void menuControl()
{
  int32_t encPos = enc.read();
  bool buttonState = digitalRead(buttonPin);

  // menu lib setup loop, runs once
  if(LCDML.BT_setup())
  {
    pinMode(encoderPinA, INPUT_PULLUP);
    pinMode(encoderPinB, INPUT_PULLUP);
    pinMode(buttonPin, INPUT_PULLUP);
  }

  if(encPos <= -3)
  {
    // check if button is pressed and encoder rotated
    if(!buttonState)
    {
      LCDML.BT_left();

      // reset button press time
      g_LCDML_CONTROL_button_prev = HIGH;
    }
    else
    {
      LCDML.BT_down();
    }
    enc.write(encPos + 4);
  }
  else if (encPos >= 3)
  {
    if(!buttonState)
    {
      LCDML.BT_right();

      // reset button press time
      g_LCDML_CONTROL_button_prev = HIGH;
    }
    else
    {
      LCDML.BT_up();
    }
    enc.write(encPos + 4);
  }
  else
  {
    // check if the button was pressed for a shortly time or a long time

    //falling edge, button pressed, no action
    if(buttonState == LOW && g_LCDML_CONTROL_button_prev == HIGH)
    {
      g_LCDML_CONTROL_button_prev = LOW;
      g_LCDML_CONTROL_button_press_time = millis();
    }

    // rising edge, button not pressed, check how long was it pressed
    else if(buttonState == HIGH && g_LCDML_CONTROL_button_prev == LOW)
    {
      g_LCDML_CONTROL_button_prev = HIGH;

      // check how long was the button pressed and detect a long press or a short press

      // check long press situation
      if((millis() - g_LCDML_CONTROL_button_press_time) >= g_LCDML_CONTROL_button_long_press)
      {
        // long press
        LCDML.BT_quit();
      }
      // check short press situation
      else if((millis() - g_LCDML_CONTROL_button_press_time) >= g_LCDML_CONTROL_button_short_press)
      {
        // short press
        LCDML.BT_enter();
      }
    }
    else
    {
      // do nothing
    }
    
  }
}

void menuClear()
{
  // clear menu function
}
