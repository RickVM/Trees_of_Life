#pragma once

#include <Arduino.h>
#include "Communication.h"

#define OldOrNew 2

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
    int sendToAll(String message);
};

