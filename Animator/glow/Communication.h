#pragma once

#include <Arduino.h>

enum COMMANDS {
  error,
  pulse,
  flash,
  backward,
  rest
};

class Communication
{
  public:

    virtual void Begin() = 0;
    virtual COMMANDS readCommand(int id) = 0;
    virtual int sendCommand(int id, String message) = 0;

    String _format(String message);
    COMMANDS checkProtocol(String message);
};

