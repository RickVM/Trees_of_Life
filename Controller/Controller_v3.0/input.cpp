#include "input.h"

int Input::formatByte(uint8_t* values)//Tested
{
  uint32_t rv = 0;
  uint32_t temp = 0;
  for (int i = 0; i < this->numInputs; i++)
  {
    temp = values[i];
    temp = temp << i;
    rv |= temp;
  }
  return rv;
}

void Input::setRequiredDistance(int x)
{
  requiredDistance = x;
}

void Input::setInput(int pos, int x)
{
  valueArray[pos] = x;
  if (x > 0 && x < requiredDistance) {
    highOrLowArray[pos] = true;
    Serial.println("Button is true");
  }
  else if (x == 0) {
    highOrLowArray[pos] = false;
    Serial.println("Button is false");
  } 
  classificationArray[pos] = 0;//Add classification code here.
}

boolean Input::getInputHigh(int pos)
{
  return highOrLowArray[pos];
}

int Input::getInputValue(int pos)
{
  return valueArray[pos];
}

int Input::getInputClassification(int pos)
{
  return classificationArray[pos];
}

