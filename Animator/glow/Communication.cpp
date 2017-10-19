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
  if (message == "pulse1")
  {
    rv = pulse1;
  }
  else if (message == "pulse2")
  {
    rv = pulse2;
  }
  else if (message == "pulse3")
  {
    rv = pulse3;
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

