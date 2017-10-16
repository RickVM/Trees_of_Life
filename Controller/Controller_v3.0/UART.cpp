#include "UART.h"

UART::UART(uint8_t port, uint32_t baudrate) : Communication()
{
  this->_baudRate = baudrate;
  this->_port = port;
}

void UART::Begin()
{
  switch (this->_port) {
    case 1:
      Serial1.begin(this->_baudRate);
      break;
    case 2:
      Serial2.begin(this->_baudRate);
      break;
    case 3:
      Serial3.begin(this->_baudRate);
      break;
    default:
      //Set error value, not inplemented yet
      break;
  }
}

//Not needed for first prototype
COMMANDS UART::readCommand()
{
  COMMANDS rv = error;
  switch (this->_port) {
    case 1:
      if (Serial1.available() > 0)
      {
        this->_readByte = Serial1.read();
        _receivedChar = (char)_readByte;
        if (_receivedChar == '#') //Received start of message
        {
          this->_command = true;
          msg = "";
        }
        else if (_receivedChar == '@')//Received end of message
        {
          this->_command = false;
          rv = checkProtocol();
        }
        else if (this->_command == true)
        {
          msg = msg + _receivedChar;
        }
      }
      break;
    case 2:
      break;
    case 3:
      break;
    default:
      //Set error value, not inplemented yet
      break;
  }
  return rv;
}

int UART::sendCommand(String message)
{
  int rv = 0;
  switch (this->_port) {
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
