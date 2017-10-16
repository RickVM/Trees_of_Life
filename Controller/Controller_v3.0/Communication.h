#pragma once

#include <Arduino.h>

enum COMMANDS {
  error,
  pulse,
  flash,
  backward
};

class Communication
{
  public:
    bool _command;
    String msg;

    virtual void Begin() = 0;
    virtual COMMANDS readCommand() = 0;
    virtual int sendCommand(String message) = 0;

    String _format(String message);
    COMMANDS checkProtocol();
};

