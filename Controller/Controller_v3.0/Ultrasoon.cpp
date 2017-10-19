#include "Ultrasoon.h"

Ultrasoon::Ultrasoon(uint8_t numInputs, uint8_t* trigPins, uint8_t* echoPins) : Input()
{
  this->numInputs = numInputs;
  this->trigPins = trigPins;
  this->echoPins = echoPins;
}

Ultrasoon::~Ultrasoon(void)
{
  //Not inplemented
}

void Ultrasoon::Begin(void)
{
  for (int i = 0; i < this->numInputs; i++)
  {
    pinMode(this->trigPins[i], OUTPUT);
    pinMode(this->echoPins[i], INPUT);
  }
  this->setRequiredDistance(11);
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
    setInput(i, ((duration[i] / 2) / 29.1));
  }
  Serial.print("The ultrasoon values are: ");
  Serial.print(duration[0]);
  Serial.print(", ");
  Serial.println(duration[1]);
  
  
}

void Ultrasoon::readInputs(void)
{
  PingDistance();
}


