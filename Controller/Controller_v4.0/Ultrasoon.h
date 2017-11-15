#pragma once

#include <Arduino.h>
#include <NewPing.h>
#include "Input.h"


#define useTemprature 0

#define SONAR_NUM 6         //Amount of sensors
#define MAX_DISTANCE 100    //Max distance to ping
#define PING_INTERVAL 33    //Milliseconds between pings, 29ms min

class Ultrasoon : public Input
{
  private:
    uint8_t* trigPins;
    uint8_t* echoPins;
    uint8_t* distances;
    void PingDistance(void);

    unsigned long pingTimer[SONAR_NUM];
    unsigned int cm[SONAR_NUM];
    uint8_t currentSensor;

    NewPing sonar[SONAR_NUM] {
      NewPing(22, 23, MAX_DISTANCE),
      NewPing(20, 21, MAX_DISTANCE),
      NewPing(18, 19, MAX_DISTANCE),
      NewPing(16, 17, MAX_DISTANCE),
      NewPing(14, 15, MAX_DISTANCE),
      NewPing(11, 12, MAX_DISTANCE)
    };

  public:
    Ultrasoon(uint8_t numInputs, uint8_t* trigPins, uint8_t* echoPins);
    ~Ultrasoon(void);
    void Begin(void);
    void readInputs(void);//Return if reading inputs was succesfull.

    void sensorLoop();
    void oneSensorCycle();
    static void echoCheck();
};
