#include "Button.h"

//Public functions

Button::Button(uint8_t numinputs, uint8_t* arrayinputs) : Input()
{
  this->numInputs = numinputs;
  this->arrayinputs = arrayinputs;
}

Button::~Button()
{
  //Not inplemented at the moment
}

void Button::Begin()
{
  Serial.println("Button begin.");
  for (int i = 0; i < this->numInputs; i++)
  {
    pinMode(this->arrayinputs[i], INPUT_PULLUP);
  }
}

void Button::readInputs()
{
  Serial.println("Button read inputs.");
  for (int i = 0; i <= this->numInputs; i++)
  {
    this->setInput(i, digitalRead(this->arrayinputs[i]));
  }
}

