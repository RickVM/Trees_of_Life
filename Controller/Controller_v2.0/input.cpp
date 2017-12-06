#include "input.h"

//Public functions

Input::Input(uint8_t numinputs, uint8_t* arrayinputs)
{
  this->numinputs = numinputs;
  this->arrayinputs = arrayinputs;
}

void Input::Begin()
{
  for(int i = 0; i < this->numinputs; i++)
  {
    pinMode(this->arrayinputs[i], INPUT_PULLUP);
  }
}

int Input::readInputs()
{
  uint8_t temp[this->numinputs] = {};
  for(int i = 0; i <= this->numinputs; i++)
  {
    temp[i] = digitalRead(this->arrayinputs[i]);//Or a read function for ultrasoon
  }
  return formatByte(temp);
}

//Private functions

int Input::formatByte(uint8_t* values)//Tested
{
  uint32_t rv = 0;
  uint32_t temp = 0;
  for(int i = 0; i < this->numinputs; i++)
  {
    temp = values[i];
    temp = temp << i;
    rv |= temp;
  }
  return rv; 
}

