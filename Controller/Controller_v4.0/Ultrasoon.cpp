#include "Ultrasoon.h"

#define useLibrary 0

Ultrasoon::Ultrasoon(uint8_t numInputs, uint8_t* trigPins, uint8_t* echoPins) : Input()
{
  this->numInputs = numInputs;
  this->trigPins = trigPins;
  this->echoPins = echoPins;
  this->inputMethode = 2;
  currentSensor = 0;
}

Ultrasoon::~Ultrasoon(void)
{
  //Not inplemented
}

void Ultrasoon::Begin(void)
{
  switch (useLibrary) {
    case 0:
      for (int i = 0; i < this->numInputs; i++)
      {
        pinMode(this->trigPins[i], OUTPUT);
        pinMode(this->echoPins[i], INPUT);
      }
      this->setRequiredDistance(11, 1);
      break;
    case 1:
      //Setup function
      pingTimer[0] = millis() + 75;
      for (int i = 1; i < SONAR_NUM; i++)
      {
        pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
      }
      break;
    default:
      //Not inplemented
      break;
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
  //delayMicroseconds(10); // Added this line

  for (int i = 0 ; i < numInputs; i++) {
    digitalWrite(trigPins[i], LOW);
    duration[i] = pulseIn(echoPins[i], HIGH);
    setInput(i, ((duration[i] / 2) / 29.1));
  }
  
/*    Serial.print("The ultrasoon cm values are: ");
    Serial.print(getInputValue(0));
    Serial.print(", ");
    Serial.println(getInputValue(1));
*/}

void Ultrasoon::sensorLoop()
{
  for (uint8_t i = 0; i < SONAR_NUM; i++)
  {
    if (millis() >= pingTimer[i]) {
      pingTimer[i] += PING_INTERVAL * SONAR_NUM;
      if (i == 0 && currentSensor == SONAR_NUM - 1) oneSensorCycle();
      sonar[currentSensor].timer_stop();
      currentSensor = i;
      cm[currentSensor] = 0;
   //   sonar[currentSensor].ping_timer(Ultrasoon::echoCheck);
    }
  }
}

void Ultrasoon::readInputs(void)
{
  switch (useLibrary) {
    case 0:
      PingDistance();
      break;
    case  1:
      //Ping the distance while using a library
      sensorLoop();
      break;
    default:
      //Not inplemented
      break;
  };
}

void Ultrasoon::oneSensorCycle() {
  //Save the ultrasonic values into the arrays
  for (uint8_t i = 0; i < SONAR_NUM; i++)
  {
    setInput(i, cm[i]);
  }
}
/*
void Ultrasoon::echoCheck()
{
  if (sonar[currentSensor].check_timer())
    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
}*/
