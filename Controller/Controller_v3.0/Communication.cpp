#include "Communication.h"

String Communication::_format(String message)
{
  String temp = "#";
  temp.concat(message);
  temp.concat('@');
  return temp;
}

COMMANDS Communication::checkProtocol()
{
  COMMANDS rv = error;
  if(msg == "pulse")
  {
    rv = pulse;
  }
  else if(msg == "flash")
  {
    rv = flash;
  }
  else if(msg == "backward")
  {
    rv = backward;
  }
  msg = ""; 
  return rv; 
}

