#pragma once

#include <Arduino.h>
#include "Communication.h"

class UART : public Communication
{
  private:
    int _baudRate;
    bool _command;
    String msg;
    int _readByte;
    char _receivedChar;

  public:
    UART(int baudrate);
    void Begin();
    COMMANDS readCommand(int id);
    int sendCommand( int id, String message);
};

