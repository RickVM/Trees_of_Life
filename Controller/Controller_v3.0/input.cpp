#include "input.h"

/*
   Not used anymore at the moment
   Expects an array of uint8_t
   Returns an uint32_t that contains the values in the format of 001011
   Meaning array[0] = 1, array[1] = 1, array[2] = 0
*/
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

/*
   ALWAYS set this distance to minimum of 2
   otherwise input will not work.
*/
void Input::setRequiredDistance(int x, int y)
{
  requiredDistance = x;
  minimalDistance = y;
}

/*
   Function for setting the values of the three arrays
   that hold all the information of the input
*/
void Input::setInput(int pos, int x)
{
  valueArray[pos] = x;
  if (x > minimalDistance && x < requiredDistance) {
    highOrLowArray[pos] = true;
    if (x < 6)
    {
      classificationArray[pos] = 1;//Add classification code here.
    }
    else if (x < 8)
    {
      classificationArray[pos] = 2;//Add classification code here.
    }
    else
    {
      classificationArray[pos] = 3;//Add classification code here.
    }
  }
  else
  {
    highOrLowArray[pos] = false;
  }
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

int Input::getMethode(void)
{
  return this->inputMethode;
}

