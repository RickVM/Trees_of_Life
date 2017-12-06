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
  Serial.print("Using mode: ");
  Serial.println(OldOrNew);
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

/*
   ID 1 & 2, 3 & 4, 5 & 6 go to the same teensy
   Format message like #1pulse

*/

int UART::sendCommand(int id, String message)
{
  int rv = 0;
  String tempMessage = "";
  switch (OldOrNew) {
    case 1: //Old
      switch (id) {
        case 1:
          Serial1.println(_format(message));
          Serial.print("Sende message to teensy 1: ");
          Serial.println(message);
          break;
        case 2:
          Serial2.println(_format(message));
          Serial.print("Sende message to teensy 2: ");
          Serial.println(message);
          break;
        case 3:
          Serial3.println(_format(message));
          Serial.print("Sende message to teensy 3: ");
          Serial.println(message);
          break;
        default:
          rv = -1;
          //Set error value, not inplemented yet
          break;
      }
      break;
    case 2: //New
      switch (id) {
        case 1:
          //Send to teensy 1
          tempMessage = "1";
          tempMessage.concat(message);
          Serial1.println(_format(tempMessage));
          Serial.print("Sended = ");
          Serial.print(tempMessage);
          Serial.println(" to teensy 1.");
          break;
        case 2:
          //Send to teensy 1
          tempMessage = "2";
          tempMessage.concat(message);
          Serial1.println(_format(tempMessage));
          Serial.print("Sended = ");
          Serial.print(tempMessage);
          Serial.println(" to teensy 1.");
          break;
        case 3:
          //Send to teensy 2
          tempMessage = "1";
          tempMessage.concat(message);
          Serial2.println(_format(tempMessage));
          Serial.print("Sended = ");
          Serial.print(tempMessage);
          Serial.println(" to teensy 2.");
          break;
        case 4:
          //Send to teensy 2
          tempMessage = "2";
          tempMessage.concat(message);
          Serial2.println(_format(tempMessage));
          Serial.print("Sended = ");
          Serial.print(tempMessage);
          Serial.println(" to teensy 2.");
          break;
        case 5:
          //Send to teensy 3
          tempMessage = "1";
          tempMessage.concat(message);
          Serial3.println(_format(tempMessage));
          Serial.print("Sended = ");
          Serial.print(tempMessage);
          Serial.println(" to teensy 3.");
          break;
        case 6:
          //Send to teensy 3
          tempMessage = "2";
          tempMessage.concat(message);
          Serial3.println(_format(tempMessage));
          Serial.print("Sended = ");
          Serial.print(tempMessage);
          Serial.println(" to teensy 3.");
          break;
        default:
          //Not inplemented
          break;
      }
      break;
    default:
      //Not inplemented
      break;
  }
  
  return rv;
}
