#include "I2C.h"

I2C::I2C(int id)
{
  this->id = id;
  receivedMessage = "";
  received = false;
}

I2C::~I2C(void)
{

}

void I2C::Begin()
{
  Wire.begin(this->id);
  Wire.onReceive(receiveEvent);
}

COMMANDS I2C::readCommand(int id)
{
  COMMANDS rv = error;
  if (received)
  {
    cli();
    rv = checkProtocol(receivedMessage);
    received = false;
    receivedMessage = "";
    sei();
  }
  return rv;
}

int I2C::sendCommand(int id, String message)
{
  int rv = 0;
  char _buffer[32];
  String temp = _format(message);
  temp.toCharArray(_buffer, 32);

  Wire.beginTransmission(id);
  Wire.write(_buffer);
  Wire.endTransmission();
  return rv;
}

void I2C::receiveEvent(int howMany)
{
  bool _command = false;
  String msg = "";
  int _readByte = 0;
  char _receivedChar = 0;

  while (Wire.available() > 0)
  {
    _readByte = Wire.read();
    _receivedChar = (char) _readByte;
    if (_receivedChar == '#')
    {
      _command = true;
      msg = "";
    }
    else if (_receivedChar == '@')
    {
      _command = false;
      receivedMessage = msg;
      received = true;
    }
    else if (_command == true)
    {
      msg = msg + _receivedChar;
    }
  }
}

