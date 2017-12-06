#include <Arduino.h>
#include "UART.h"
#include "I2C.h"

#define ID 1

Communication* COM;

void setup() {
  Serial.begin(9600);
  COM = new I2C(ID);
  COM->Begin();
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

}

void loop() {
  COMMANDS rv = COM->readCommand(1);
  switch (rv)
  {
    case pulse:
      Serial.println("Received a pulse");
      break;
    case flash:
      Serial.println("Received flash");
      break;
  }
}
