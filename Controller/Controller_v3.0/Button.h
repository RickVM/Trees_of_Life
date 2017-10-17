#pragma once

#include <Arduino.h>
#include "input.h"

class Button : public Input
{
  private:
    uint8_t* arrayinputs;//Pointer to an global array, tested
    
  public:
    //Constructor sets the number of intputs and which pins that can be found as a global array.
    Button(uint8_t numinputs, uint8_t* arrayinputs);
    ~Button();
    void Begin();
    void readInputs();
};

