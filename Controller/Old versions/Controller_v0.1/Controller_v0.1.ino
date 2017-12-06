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
uint8_t inputsArray[NUM_INPUTS] = {10, 11, 12, 13, 14, 15};//Change the numbers, but DO NOT DELETE THIS ARRAY

//Locale vars
uint32_t _inputs;

void setup() {
  Serial.begin(9600);//Just for debugging.
  S1 = new Communication(1, BAUD_RATE);
  S2 = new Communication(2, BAUD_RATE);
  S3 = new Communication(3, BAUD_RATE);
  input = new Input(NUM_INPUTS, inputsArray);
  _inputs = 0;
}

void loop() 
{
  _inputs = input->readInputs();//Read inputs
  if(_inputs > 0)//A input is high (or more)
  {
      
  }
  else
  {
    //Nothing inplemented, idle state
  }
}

/*Logic of program:
 *  Read inputs  
 *  Send when a pulse is needed, keep track off how many per second, a timer per pulse you send
 *  Loop until all inputs are HIGH
 *  Adjust when the pulses are given so that they start syncen
 *  Finish syncen for a set number of pulses
 *  Glow tree
 *  Wait a certain amount of time and restart.
 */
