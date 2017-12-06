#pragma once

#include <Arduino.h>

class Communication
{
  private:
    int baudRate;
    int port;

    String _format(String message);//Function tested
    
  public:
    Communication(uint8_t port, uint32_t baudrate); 
    void Begin();
    String readSerialPort();
    uint8_t sendMessage(String message); 
};

