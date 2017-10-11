/*
 * Changes sinds version 2.0
 * - Added actual syncing 
 * - Added ultrasoon class
 */
//Inlcudes
#include "input.h"
#include "Controller.h"

//Defines
#define NUM_SLAVES 3
#define NUM_LEDSTRIPS 4 //per slave
#define NUM_INPUTS 2//Per controller

//Objects
Input* input;
Ultrasoon* ultrasoon;

Controller* c;

uint8_t inputsArray[NUM_INPUTS] = {14, 15};//Change the numbers, but DO NOT DELETE THIS ARRAY
uint8_t triggerArray[NUM_INPUTS] = {16, 17};
uint8_t distances[NUM_INPUTS]; //Put this in ultrasoon class later;
//uint32_t _inputs;
uint8_t* _inputs;

void setup() {
  Serial.begin(9600);//Just for debugging.
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  //input = new Input(NUM_INPUTS, inputsArray);
  ultrasoon = new Ultrasoon(NUM_INPUTS, triggerArray, inputsArray, distances);
  ultrasoon->Begin();
  //_inputs = 0;
  _inputs = NULL;
  
  c = new Controller();
  c->Begin();
}

void loop()
{
  _inputs = ultrasoon->ReadInputs();
  c->Logic1(_inputs);
}

