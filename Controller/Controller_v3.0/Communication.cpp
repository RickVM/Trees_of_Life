#include "Communication.h"

String Communication::_format(String message)
{
  String temp = "#";
  temp.concat(message);
  temp.concat('@');
  return temp;
}

COMMANDS Communication::checkProtocol(String message)
{
  COMMANDS rv = error;
  if (message == "pulse5")
  {
    rv = pulse5;
  }
  else if (message == "pulse6")
  {
    rv = pulse6;
  }
  else if (message == "pulse7")
  {
    rv = pulse7;
  }
  else if (message == "pulse8")
  {
    rv = pulse8;
  }
  else if (message == "pulse9")
  {
    rv = pulse9;
  }
  else if (message == "pulse10")
  {
    rv = pulse10;
  }
  else if (message == "pulse11")
  {
    rv = pulse11;
  }
  else if (message == "flash")
  {
    rv = flash;
  }
  else if (message == "backward")
  {
    rv = backward;
  }
  else if (message == "rest")
  {
    rv = rest;
  }
  message = "";
  return rv;
}

