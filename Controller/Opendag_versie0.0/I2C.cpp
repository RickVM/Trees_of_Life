#include "I2C.h"

//Vars needed by the onreceive event
String receivedMessage;
boolean received;

//Function prototype
void receiveEvent(int howMany);

I2C::I2C(int id)
{
  this->id = id;
  receivedMessage = "";
  received = false;
}

I2C::~I2C(void)
{

}

/*
 * Setup of I2C
 */
void I2C::Begin()
{
  Wire.begin();
  //Needed for teensy
//  Wire.setSDA(18);
//  Wire.setSCL(19);
  Wire.onReceive(receiveEvent);
}

/*
 * Function that reads if there has been an new message received
 * if a new one has been received it gets checked against the protocol 
 * and returnd to the caller of the function
 */
COMMANDS I2C::readCommand(int id)
{
  COMMANDS rv = error;
  if (received)
  {
    rv = checkProtocol(receivedMessage);
    received = false;
    receivedMessage = "";
  }
  return rv;
}

/*
 * Function to send a message over the I2C bus
 * needs a buffer because the wire library does not no the type String
 * expects an string of type char array.
 */
int I2C::sendCommand(int id, String message)
{
  int rv = 0;
  char _buffer[32];
  String temp = _format(message);
  temp.toCharArray(_buffer, 32);
  Wire.beginTransmission(id);
  Wire.write(_buffer);
  Wire.endTransmission();
  Serial.print("Sending message: ");
  Serial.print(temp);
  Serial.print(" ,Sending to ");
  Serial.println(id);
  return rv;
}

/*
 * Function that get's trigger when ever a new message is received
 */
void receiveEvent(int howMany)
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

