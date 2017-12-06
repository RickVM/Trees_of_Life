#include "Communication.h"

//Public functions

Communication::Communication(uint8_t port, uint32_t baudrate)
{
  this->baudRate = baudrate;
  this->port = port;
}

void Communication::Begin()
{
  switch(this->port){
    case 1:
      Serial1.begin(this->baudRate);
      break;
      case 2:
       Serial2.begin(this->baudRate);
        break;
        case 3: 
        Serial3.begin(this->baudRate);
          break;
          default:
            //Set error value, not inplemented yet
            break;
  }
}

//Not needed for first prototype
String Communication::readSerialPort()
{
  switch(this->port){
    case 1:
      break;
      case 2:
        break;
        case 3: 
          break;
          default:
            //Set error value, not inplemented yet
            break;
  }
  return "ERROR";
}

uint8_t Communication::sendMessage(String message)
{
  uint8_t rv = 0;
  switch(this->port){
    case 1:
      Serial1.println(_format(message));
      break;
      case 2:
       Serial2.println(_format(message));
        break;
        case 3: 
        Serial3.println(_format(message));
          break;
          default:
            rv = -1;
            //Set error value, not inplemented yet
            break;
  }
  return rv;
}

//Private functions
String Communication::_format(String message)
{
  String temp = "#";
  temp.concat(message);
  temp.concat('@'); 
  return temp;
}

