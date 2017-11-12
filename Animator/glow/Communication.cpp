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
  //Strip message of number
  String h = message.substring(0, 1);
  this->hand = h.toInt();
  //Save number as hand var
  String tempMessage = message.substring(1);
  if (tempMessage == "pulse5")
  {
    rv = pulse5;
  }
  else if (tempMessage == "pulse6")
  {
    rv = pulse6;
  }
  else if (tempMessage == "pulse7")
  {
    rv = pulse7;
  }
  if (tempMessage == "pulse8")
  {
    rv = pulse8;
  }
  else if (tempMessage == "pulse9")
  {
    rv = pulse9;
  }
  else if (tempMessage == "pulse10")
  {
    rv = pulse10;
  }
  else if (tempMessage == "pulse11")
  {
    rv = pulse11;
  }
  else if (tempMessage == "flash")
  {
    rv = flash;
  }
  else if (tempMessage == "backward")
  {
    rv = backward;
  }
  else if (tempMessage == "rest")
  {
    rv = rest;
  }
  message = "";
  return rv;
}

int Communication::getHand(void)
{
  return this->hand;
}

