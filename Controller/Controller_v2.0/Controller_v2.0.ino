//Inlcudes
#include "input.h"
#include "Controller.h"

//Defines
#define NUM_SLAVES 3
#define NUM_LEDSTRIPS 4 //per slave
#define NUM_INPUTS 2//Per controller

//Objects
Input* input;

Controller* c;

uint8_t inputsArray[NUM_INPUTS] = {14, 15};//Change the numbers, but DO NOT DELETE THIS ARRAY

uint32_t _inputs;

void setup() {
  Serial.begin(9600);//Just for debugging.
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  
  input = new Input(NUM_INPUTS, inputsArray);
  input->Begin();
  _inputs = 0;
  
  c = new Controller();
  c->Begin();
}

void loop()
{
  _inputs = input->readInputs();
  c->Logic(_inputs);
}

