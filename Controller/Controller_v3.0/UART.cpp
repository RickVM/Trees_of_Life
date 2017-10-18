#include "UART.h"

UART::UART(int baudrate) : Communication()
{
  this->_baudRate = baudrate;
}

void UART::Begin()
{
  Serial1.begin(this->_baudRate);
  Serial2.begin(this->_baudRate);
  Serial3.begin(this->_baudRate);
}

//Not needed for first prototype
COMMANDS UART::readCommand(int id)
{
  COMMANDS rv = error;
  switch (id) {
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
          rv = checkProtocol(msg);
        }
        else if (this->_command == true)
        {
          msg = msg + _receivedChar;
        }
      }
      break;
    case 2:
      if (Serial2.available() > 0)
      {
        this->_readByte = Serial2.read();
        _receivedChar = (char)_readByte;
        if (_receivedChar == '#') //Received start of message
        {
          this->_command = true;
          msg = "";
        }
        else if (_receivedChar == '@')//Received end of message
        {
          this->_command = false;
          rv = checkProtocol(msg);
        }
        else if (this->_command == true)
        {
          msg = msg + _receivedChar;
        }
      }
      break;
    case 3:
      if (Serial3.available() > 0)
      {
        this->_readByte = Serial3.read();
        _receivedChar = (char)_readByte;
        if (_receivedChar == '#') //Received start of message
        {
          this->_command = true;
          msg = "";
        }
        else if (_receivedChar == '@')//Received end of message
        {
          this->_command = false;
          rv = checkProtocol(msg);
        }
        else if (this->_command == true)
        {
          msg = msg + _receivedChar;
        }
      }
      break;
    default:
      //Set error value, not inplemented yet
      break;
  }
  return rv;
}

int UART::sendCommand(int id, String message)
{
  int rv = 0;
  switch (id) {
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
  Serial.print("Sended message to: ");
  Serial.println(id);
  return rv;
}
