//Inlcudes
#include "input.h"
#include "Button.h"
#include "Ultrasoon.h"
#include "Communication.h"
#include "UART.h"
#include "I2C.h"
#include "Controller.h"
#include <Arduino.h>

//Defines
#define INPUT_METHOD 2// 1 for buttons, two for 
#define COMMUNICATION_METHOD 2 // 1 for serial, 2 for i2c
#define NUM_ANIMATORS 3
#define NUM_INPUTS 3
#define ID 0

//Objects
Input* input;
Controller* c;

//Input arrays
uint8_t inputsArray[6] = {14, 15, 16, 17, 20, 21};//Change the numbers, but DO NOT DELETE THIS ARRAY
uint8_t triggerArray[6] = {12, 11, 6, 4, 3, 2};

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
  delay(1);
}

