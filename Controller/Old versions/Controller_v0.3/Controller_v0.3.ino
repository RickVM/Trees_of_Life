//Program for controling multiple teensy's that each control a set number of led strips
//Serial 1 = 0,1, serial 2 = 9,10, serial 3 = 7,8

//Inlcudes
#include "Communication.h"
#include "input.h"

//Defines
#define NUM_SLAVES 3
#define NUM_LEDSTRIPS 4 //per slave
#define NUM_INPUTS 6//Per controller
#define BAUD_RATE 9600

//Objects
Communication* S1;
Communication* S2;
Communication* S3;
Input* input;

//Global vars
uint8_t inputsArray[NUM_INPUTS] = {14, 15, 16, 17, 18, 19};//Change the numbers, but DO NOT DELETE THIS ARRAY
uint8_t testLedArray[NUM_INPUTS] = {20, 21, 22, 23, 2, 3};

//Locale vars
uint32_t _inputs;
unsigned long _currentTime;
unsigned long _oldTime[NUM_INPUTS] = {0, 0, 0, 0, 0, 0};
unsigned long pulsePeriod[NUM_INPUTS] = {500, 1000, 500, 1000, 500, 1000};
boolean pulsing[NUM_INPUTS] = {false, false, false, false, false, false};
boolean _startSync;

void setup() {
  Serial.begin(9600);//Just for debugging.
  for (int i = 0; i < NUM_INPUTS; i++)
  {
    pinMode(testLedArray[i], OUTPUT);
  }
  S1 = new Communication(1, BAUD_RATE);
  S2 = new Communication(2, BAUD_RATE);
  S3 = new Communication(3, BAUD_RATE);
  input = new Input(NUM_INPUTS, inputsArray);
  input->Begin();
  _inputs = 0;
  _startSync = false;
  _currentTime = millis();
}

void loop()
{
  _inputs = input->readInputs();//Read inputs
  if (_inputs > 0) //A input is high (or more)
  {
    //Reset all, so when somebody lets go will notice
    for(int i = 0; i < NUM_INPUTS; i++)
    {
      pulsing[i]= false;
    }
    _currentTime = millis();
    if ((_inputs & (1 << 0)) == 1)
    {
      if (_currentTime - _oldTime[0] > pulsePeriod[0])
      {
        Serial.println("First");
        _oldTime[0] = _currentTime;
        pulsing[0]= true;
      }
    }
    if ((_inputs & (1 << 1)) == 2)
    {
      if (_currentTime - _oldTime[1] > pulsePeriod[1])
      {
        digitalWrite(testLedArray[1], !digitalRead(testLedArray[1]));
        _oldTime[1] = _currentTime;
        pulsing[1]= true;
      }
    }
    if ((_inputs & (1 << 2)) == 4)
    {
      if (_currentTime - _oldTime[2] > pulsePeriod[2])
      {
        digitalWrite(testLedArray[2], !digitalRead(testLedArray[2]));
        _oldTime[2] = _currentTime;
        pulsing[2]= true;
      }
    }
    if ((_inputs & (1 << 3)) == 8)
    {
      if (_currentTime - _oldTime[3] > pulsePeriod[3])
      {
        digitalWrite(testLedArray[3], !digitalRead(testLedArray[3]));
        _oldTime[3] = _currentTime;
        pulsing[3]= true;
      }
    }
    if ((_inputs & (1 << 4)) == 16)
    {
      if (_currentTime - _oldTime[4] > pulsePeriod[4])
      {
        digitalWrite(testLedArray[4], !digitalRead(testLedArray[4]));
        _oldTime[4] = _currentTime;
        pulsing[4]= true;
      }
    }
    if ((_inputs & (1 << 5)) == 32)
    {
      if (_currentTime - _oldTime[5] > pulsePeriod[5])
      {
        digitalWrite(testLedArray[5], !digitalRead(testLedArray[5]));
        _oldTime[5] = _currentTime;
        pulsing[5]= true;
      }
    }
    //Check if all pulses are active, if start adjusting the pulsing time
    checkSync();
    if(_startSync)
    {
      for(int i = 0; i < NUM_INPUTS; i++)
      {
        _oldTime[i] = 0;
      }
    }
  }
  else
  {
    //Nothing inplemented, idle state
  }
  
}

//Check if all inputs are pressed
void checkSync()
{
  if(pulsing[0] == 1 && pulsing[1] == 1 &&pulsing[2] == 1 &&
  pulsing[3] == 1 && pulsing[4] == 1 && pulsing[5] == 1)
  {
    _startSync = true;
  }
  else
  {
    _startSync = false;
  }
}


/*Logic of program:
    Read inputs
    Send when a pulse is needed, keep track off how many per second, a timer per pulse you send
    Loop until all inputs are HIGH
    Adjust when the pulses are given so that they start syncen
    Finish syncen for a set number of pulses
    Glow tree
    Wait a certain amount of time and restart.
*/
