#include "lcdMenu.h"
#include <LCDMenuLib2.h>
// global variables for lcd menu system
unsigned long  g_LCDML_CONTROL_button_press_time = millis();
bool  g_LCDML_CONTROL_button_prev       = HIGH;


/* ******************************************************************** */
void menuControl()
/* ******************************************************************** */
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


/* ******************************************************************** */
void menuClear()
/* ******************************************************************** */
{
  // clear menu function
}

void screenSaver(uint8_t param)
{
  if(LCDML.FUNC_setup())
  {
    // remove compiler warning when param not used
    LCDML_UNUSED(param);

    // setup function
    display.clear();
    display.clearBuffer();
    display.setFont(_LCDML_DISP_font);
    display.drawStr( 0, (_LCDML_DISP_font_h * 1), "screensaver");
    display.drawStr( 0, (_LCDML_DISP_font_h * 2), "press any key");
    display.drawStr( 0, (_LCDML_DISP_font_h * 3), "to leave it");
    display.sendBuffer();

    LCDML.FUNC_setLoopInterval(100);
  }

  if(LCDML.FUNC_loop())
  {
    if(LCDML.BT_checkAny())
    {
      LCDML.FUNC_goBackToMenu();
    }
  }

  if(LCDML.FUNC_close())
  {
    // send back to the root menu
    LCDML.MENU_goRoot();
  }
}


/* ******************************************************************** */
void menuBack(uint8_t param)
/* ******************************************************************** */
{
  if(LCDML.FUNC_setup())
  {
    LCDML_UNUSED(param);

    LCDML.FUNC_goBackToMenu(1);
  }
}


/* ******************************************************************** */
void offsetParam(uint8_t line)
/* ******************************************************************** */
{
  if (line == LCDML.MENU_getCursorPos())
  {
    if(LCDML.BT_checkAny())
    {
      if(LCDML.BT_checkEnter())
      {
        // check scroll disable status
        if(LCDML.MENU_getScrollDisableStatus() == 0)
        {
          // disable the up/down can be used
          LCDML.MENU_disScroll();
        }
        else
        {
          // enable the normal scroll function
          LCDML.MENU_enScroll();
        }
        LCDML.BT_resetEnter();
      }
    }
  }
}


/* ******************************************************************** */
dynamParams::dynamParams()
/* ******************************************************************** */
{
  
}
