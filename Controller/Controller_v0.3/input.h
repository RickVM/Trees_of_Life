#pragma once

#include <Arduino.h>

class Input
{
  private:
    int numinputs;
    uint8_t* arrayinputs;//Pointer to an global array, tested
    int formatByte(uint8_t* values);
    
  public:
    //Constructor sets the number of intputs and which pins that can be found as a global array.
    Input(uint8_t numinputs, uint8_t* arrayinputs);
    void Begin();
    //Returns a int where each bit resembles an input, if the value is 0x03 then input 1 and 2 are high
    int readInputs();

};

