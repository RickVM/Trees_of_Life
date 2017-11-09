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
#define COMMUNICATION_METHOD 1 // 1 for serial, 2 for i2c
#define NUM_ANIMATORS 6
#define NUM_INPUTS 6
#define ID 0

//Objects
Input* input;
Controller* c;

/* PINOUT - WIRES
 * GND        Vin-
 * 0  RX1     AGND-
 * 1  TX1     3,3volt- 
 * 2-         23 T1
 * 3-         22 T2
 * 4-         21 T3
 * 5-         20 T4
 * 6-         19 T5
 * 7  RX3     18 T6
 * 8  TX3     17 E6
 * 9  RX2     16 E5
 * 10 TX2     15 E4
 * 11 E1      14 E3
 * 12 E2      13 Led
 */         

//Input arrays
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
  c = new Controller(input, COMMUNICATION_METHOD, NUM_ANIMATORS, ID);
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

