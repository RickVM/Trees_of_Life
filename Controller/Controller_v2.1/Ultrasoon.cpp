#include "Ultrasoon.h"

Ultrasoon::Ultrasoon(uint8_t numInputs, uint8_t* trigPins, uint8_t* echoPins)
{
  this->numInputs = numInputs;
  this->trigPins = trigPins;
  this->echoPins = echoPins;
}

void Ultrasoon::Begin(void)
{
  for (int i = 0; i < this->numinputs; i++)
  {
    pinMode(this->trigPins[i], OUTPUT);
    pinMode(this->echoPins[i], INPUT);
  }
}

void Ultrasoon::PingDistance(void)
{
  int duration[numInputs], distance[numInputs];
  for (int i = 0 ; i < numInputs; i++) {
    digitalWrite(trigPins[i], LOW);
  }
  delayMicroseconds(2); // Added this line
  
  for (int i = 0 ; i < numInputs; i++) {
    digitalWrite(trigPins[i], HIGH);
  }
  delayMicroseconds(10); // Added this line
  
  for (int i = 0 ; i < numInputs; i++) {
    digitalWrite(trigPins[i], LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration / 2) / 29.1;
  }


  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  //Sent out pings on all the sensors
  //Wait for the return
  //Read the distance from all of them
  //Save the distance in a array
}

uint8_t Ultrasoon::ReadInput(void)
{
  //Return an copy of an array of size six with ones and zeroes if the input is active or not
  return 0;
}


