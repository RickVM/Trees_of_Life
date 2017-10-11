#include "Ultrasoon.h"

Ultrasoon::Ultrasoon(uint8_t numInputs, uint8_t* trigPins, uint8_t* echoPins, uint8_t* distances)
{
  this->numInputs = numInputs;
  this->trigPins = trigPins;
  this->echoPins = echoPins;
  this->distances = distances;
}

void Ultrasoon::Begin(void)
{
  for (int i = 0; i < this->numInputs; i++)
  {
    pinMode(this->trigPins[i], OUTPUT);
    pinMode(this->echoPins[i], INPUT);
  }
}

void Ultrasoon::PingDistance(void)
{
  int duration[numInputs];
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
    duration[i] = pulseIn(echoPins[i], HIGH);
    distances[i] = (duration[i] / 2) / 29.1;
  }
}

uint8_t* Ultrasoon::ReadInputs(void)
{
  PingDistance();
  return distances;
}


