#pragma once

#include <Arduino.h>
#include "Input.h"

class Ultrasoon : public Input 
{
  private:
    uint8_t* trigPins;
    uint8_t* echoPins;
    uint8_t* distances;
    void PingDistance(void);


  public:
    Ultrasoon(uint8_t numInputs, uint8_t* trigPins, uint8_t* echoPins);
    ~Ultrasoon(void);
    void Begin(void);
    void readInputs(void);//Return if reading inputs was succesfull.
};

