#pragma once

#include <Arduino.h>
#include <i2c_t3.h>

#include "Communication.h"

#define MEM_LEN 256

class I2C : public Communication
{
  private:
    int ID;
    char databuf[MEM_LEN];

  public:
    I2C(int id);
    ~I2C(void);

    void Begin();
    COMMANDS readCommand(int id);
    int sendCommand(int id, String message);
};

