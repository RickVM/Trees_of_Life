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
  
  for (int i = 0; i < this->numInputs; i++)
  {
    buttons[i] = Bounce();
    pinMode(this->arrayinputs[i], INPUT_PULLUP);
    buttons[i].attach(this->arrayinputs[i]);
    buttons[i].interval(15);
  }
  this->setRequiredDistance(2);
}

void Button::readInputs()
{
  /*
  for (int i = 0; i < this->numInputs; i++)
  {
    this->setInput(i, digitalRead(this->arrayinputs[i]));
  }*/
  for (int i = 0; i < this->numInputs; i++)
  {
    buttons[i].update();
    this->setInput(i, buttons[i].read());
  }
}

