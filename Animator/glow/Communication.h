#pragma once

#include <Arduino.h>

enum COMMANDS{
  error,
  pulse,
  flash,
  backward
  };

class Communication
{
  private:
    int baudRate;
    int port;
    int readByte;
    char receivedChar;
    bool command;
    String msg;
 
    String _format(String message);//Function tested
    COMMANDS checkProtocol();

  public:
    Communication(uint8_t port, uint32_t baudrate);
    void Begin();
    COMMANDS readSerialPort();
    uint8_t sendMessage(String message);
};

