#pragma once
#include "main.h"

class lcdMenu
{
  public:

    lcdMenu();
    
    // lcd menu display function
    static void display();

    // lcd menu clear function
    static void clear();

    // lcd menu user input function
    static void control();
};
