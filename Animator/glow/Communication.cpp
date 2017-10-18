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
  if(message == "pulse")
  {
    rv = pulse;
  }
  else if(message == "flash")
  {
    rv = flash;
  }
  else if(message == "backward")
  {
    rv = backward;
  }
  else if(message == "rest")
  {
    rv = rest;
  }
  message = ""; 
  return rv; 
}

