#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "Communication.h"

class I2C : public Communication
{
  private:
    int id;

  public:
    I2C(int id);
    ~I2C(void);

    void Begin();
    COMMANDS readCommand(int id);
    int sendCommand(int id, String message);
};

