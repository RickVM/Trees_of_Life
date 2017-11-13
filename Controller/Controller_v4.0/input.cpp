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
  //Fix for shit
  randomSeed(analogRead(A12));
  _oldTime = 0;
  firstTimeStarted = 0;
  settedAllInputsHigh = false;
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
    if (x < 5)
    {
      classificationArray[pos] = 1;//Add classification code here.
    }
    else if (x < 6)
    {
      classificationArray[pos] = 2;//Add classification code here.
    }
    else if (x < 7)
    {
      classificationArray[pos] = 3;//Add classification code here.
    }
    else if (x < 8)
    {
      classificationArray[pos] = 4;//Add classification code here.
    }
    else if (x < 9)
    {
      classificationArray[pos] = 5;//Add classification code here.
    }
    else if (x < 10)
    {
      classificationArray[pos] = 6;//Add classification code here.
    }
    else
    {
      classificationArray[pos] = 7;//Add classification code here.
    }
  }
  else
  {
    highOrLowArray[pos] = false;
  }
}

boolean Input::getInputHigh(int pos)
{
  if (pos == 1 && highOrLowArray[0] == true)
  {
    classificationArray[1] = 1;
    return true;
  }
  else if (pos == 2 && highOrLowArray[3] == true)
  {
    classificationArray[2] = 1;
    return true;
  }
  else
  {
    return highOrLowArray[pos];
  }
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

void Input::fix()
{
  highOrLowArray[1] = true;
  classificationArray[1] = 1;
  valueArray[1] = 4;

  highOrLowArray[2] = true;
  classificationArray[2] = 1;
  valueArray[2] = 4;

}
/*
         Voor een tijdje een pulse laten doen
         Dan gewoon elke twee en een halve seconden hand laten pulsen
         Dan alle zes gelijk laten lopen voor een bepaalde tijd
         Daarna de flash doen, en hem weer in rust laten komen.
*/

/*
   setTimeArray used for setting time for when different shit needs to happen

*/
void Input::fakeInputs(long _time)
{
  //Check if all inputs are high, then keep them high until after the flash
  if (settedAllInputsHigh == true)
  {
    //Serial.println("Stuck in time");
    if (_time - firstTimeStarted >= 90000)
    {
      //reset the shit
      settedAllInputsHigh = false;
      _oldTime = _time;
      firstTimeStarted = _time;
    }
  }
  else if (_time - firstTimeStarted >= 60000 && settedAllInputsHigh == false)
  {
    Serial.println("stuck in other shit");
    //Set all inputs high
    for (int i = 0; i < 6; i++)
    {
      highOrLowArray[i] = true;
      classificationArray[i] = 1;
      valueArray[i] = 4;
    }
    settedAllInputsHigh = true;
  }
  else if (_time - _oldTime >= 4500)
  {
    int x = random(6);
    int y = random(1, 15);
    //Generate random value between 0 and 6 for wich hand goes on
    //Generate random value between 1 and 14 for pulse
    this->setInput(x, y);
    Serial.print("Setting hand = ");
    Serial.print(x);
    Serial.print(" with value ");
    Serial.println(y);
    _oldTime = _time;
  }
}


