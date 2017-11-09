#pragma once

#include <Arduino.h>

enum COMMANDS {
  error,
  pulse5,
  pulse6,
  pulse7,
  pulse8,
  pulse9,
  pulse10,
  pulse11,
  flash,
  backward,
  rest
};

class Communication
{
  private:
    int hand;
  public:

    virtual void Begin() = 0;
    virtual COMMANDS readCommand(int id) = 0;
    virtual int sendCommand(int id, String message) = 0;

    String _format(String message);
    COMMANDS checkProtocol(String message);
    int getHand(void);
};

