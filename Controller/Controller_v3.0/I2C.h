#pragma once

#include "Arduino.h"
#include "Communication.h"

class I2C : public Communication
{
  public:
    I2C(void);
    ~I2C(void);

    void Begin();
    COMMANDS readCommand();
    int sendCommand(String message);

};

