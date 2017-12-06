/*
    Program for  reading inputs and sending when wich
    controller needs to do what at the exact moment.

    PINOUT - WIRES
   GND        Vin-
   0  RX1     AGND-
   1  TX1     3,3volt-
   2-         23 E1
   3-         22 T1
   4-         21 E2
   5-         20 T2
   6-         19 E3
   7  RX3     18 T3
   8  TX3     17 E4
   9  RX2     16 T4
   10 TX2     15 E5
   11 E6      14 T5
   12 T6      13 Led
*/

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
#define INPUT_METHOD 2// 1 for buttons, two for ultrasonic sensors
#define COMMUNICATION_METHOD 1 // 1 for serial, 2 for i2c
#define NUM_ANIMATORS 3
#define NUM_INPUTS 6
#define ID 0

//Objects
Input* input;
Controller* c;

//Input arrays
//                        1   2   3   4   5   6
uint8_t inputsArray[6] = {23, 21, 19, 17, 15, 12};//Change the numbers, but DO NOT DELETE THIS ARRAY, echo, Rood kabel en blauw tape, of groen
uint8_t triggerArray[6] = {22, 20, 18, 16, 14, 11};//zwart kabel en wit tape, of groen

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
  c = new Controller(input, COMMUNICATION_METHOD, NUM_ANIMATORS, ID, NUM_INPUTS);
  c->Begin();
  delay(1000);
  Serial.println("Setup completed");
}

void loop()
{
  input->readInputs();
  c->Logic();
  delay(1);
}

