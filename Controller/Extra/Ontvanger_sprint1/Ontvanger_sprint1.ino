
#include "Communication.h"

#define BAUD_RATE 9600

//Objects
Communication* S1;

int testPin = 2;

void setup() {
  pinMode(testPin, OUTPUT);
  S1 = new Communication(1, BAUD_RATE);
  S1->Begin();
  Serial.println("Setup completed");
}

void loop()
{
  Serial.println("Setup completed");
  COMMANDS x = S1->readSerialPort();
  switch (x) {
    case error:
      break;
    case pulse:
      digitalWrite(testPin, HIGH);
      break;
    case flash:
      digitalWrite(testPin, LOW);
      break;
    case backward:
      digitalWrite(testPin, HIGH);
      delay(1000);
      digitalWrite(testPin, LOW);
      break;
    case reset:
      break;
  };
}

