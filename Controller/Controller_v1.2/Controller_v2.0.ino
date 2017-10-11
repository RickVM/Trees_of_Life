//Program for controling multiple teensy's that each control a set number of led strips
//Serial 1 = 0,1, serial 2 = 9,10, serial 3 = 7,8

//Inlcudes
#include "Communication.h"
#include "input.h"

//Defines
#define NUM_SLAVES 3
#define NUM_LEDSTRIPS 4 //per slave
#define NUM_INPUTS 6//Per controller
#define BAUD_RATE 57600

//Objects
Communication* S1;
Communication* S2;
Communication* S3;
Input* input;

//Global vars
uint8_t inputsArray[NUM_INPUTS] = {14, 15, 16, 17, 18, 19};//Change the numbers, but DO NOT DELETE THIS ARRAY

//Locale vars
uint32_t _inputs;
unsigned long _currentTime;
unsigned long _oldTime[NUM_INPUTS] = {0, 0, 0, 0, 0, 0};
unsigned long pulsePeriod[NUM_INPUTS] = {1500, 1500, 1500, 1500, 1500, 1500};
boolean pulsing[NUM_INPUTS] = {false, false, false, false, false, false};
boolean _startSync;

void setup() {
  Serial.begin(9600);//Just for debugging.
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  S1 = new Communication(1, BAUD_RATE);
  S2 = new Communication(2, BAUD_RATE);
  S3 = new Communication(3, BAUD_RATE);
  S1->Begin();
  S2->Begin();
  S3->Begin();
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
    for (int i = 0; i < NUM_INPUTS; i++)
    {
      pulsing[i] = false;
    }
    _currentTime = millis();
    if ((_inputs & (1 << 0)) == 1)
    {
      if (_currentTime - _oldTime[0] > pulsePeriod[0])
      {
        //Sent pulse to the first led controller
        Serial.println("First button");
        S1->sendMessage("pulse");
        _oldTime[0] = _currentTime;
        pulsing[0] = true;
      }
    }
    if ((_inputs & (1 << 1)) == 2)
    {
      if (_currentTime - _oldTime[1] > pulsePeriod[1])
      {
        //Sent pulseto the second led controller
        Serial.println("Second button");
        S2->sendMessage("pulse");
        _oldTime[1] = _currentTime;
        pulsing[1] = true;
      }
    }
    if ((_inputs & (1 << 2)) == 4)
    {
      delay(60);
      if ((_inputs & (1 << 2)) == 4)
      {
        Serial.println("Third button");
        S1->sendMessage("flash");
        S2->sendMessage("flash");
        delay(12000);//Delay temp
      }
    }
    if ((_inputs & (1 << 3)) == 8)
    {
      delay(60);
      if ((_inputs & (1 << 3)) == 8)
      {
        Serial.println("Fourth button");
        S1->sendMessage("backward");
        S2->sendMessage("backward");
      }
      delay(1000);
    }
  
  //Check if all pulses are active, if start adjusting the pulsing time
  checkSync();
  if (_startSync)
  {
    for (int i = 0; i < NUM_INPUTS; i++)
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
  if (pulsing[0] == 1 && pulsing[1] == 1)
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
