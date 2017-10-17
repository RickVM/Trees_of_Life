//Inlcudes
#include "input.h"
#include "Button.h"
#include "Ultrasoon.h"
#include "Communication.h"
#include "UART.h"
#include "I2C.h"
#include "Controller.h"

//Defines
#define INPUT_METHOD 1 // 1 for buttons, two for 
#define COMMUNICATION_METHOD 1 // 1 for serial, 2 for i2c
#define NUM_ANIMATORS 2
#define NUM_INPUTS 2//Per controller
#define ID 0

//Objects
Input* input;
Controller* c;

//Input arrays
uint8_t inputsArray[NUM_INPUTS] = {14, 15};//Change the numbers, but DO NOT DELETE THIS ARRAY
uint8_t triggerArray[NUM_INPUTS] = {17, 16};

void setup()
{
  Serial.begin(9600);//Just for debugging.
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  switch (INPUT_METHOD)
  {
    case 1:
      input = new Button(NUM_INPUTS, inputsArray);
      break;
    case 2:
      input = new Ultrasoon(NUM_INPUTS, triggerArray, inputsArray);
      break;
  };
  input->Begin();
  c = new Controller(input, COMMUNICATION_METHOD, NUM_ANIMATORS, ID);
  c->Begin();
}

void loop()
{
  input->readInputs();
  c->Logic();
}

