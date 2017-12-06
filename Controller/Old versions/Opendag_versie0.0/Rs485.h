#pragma once

#include <Arduino.h>
#include "Communication.h"

class Rs485 : public Communication
{
  private:
  public:
    Rs485();
    void Begin();
    COMMANDS readCommand(int id);
    int sendCommand( int id, String message);
};

