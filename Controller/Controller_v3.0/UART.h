#pragma once

#include <Arduino.h>
#include "Communication.h"

class UART : public Communication
{
  private:
    int _baudRate;
    int _port;
    int _readByte;
    char _receivedChar;

  public:
    UART(uint8_t port, uint32_t baudrate);
    void Begin();
    COMMANDS readCommand();
    int sendCommand(String message);
};

