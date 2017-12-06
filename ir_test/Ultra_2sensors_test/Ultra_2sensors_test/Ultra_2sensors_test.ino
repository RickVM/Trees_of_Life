//#include <DistanceGP2Y0A21YK.h>

//Arduino

#define echoPin0 11
#define trigPin0 12
#define echoPin1 14
#define trigPin1 15
#define DELAY 400

//Teensy
/*
#define echoPin0 20
#define trigPin0 21
#define echoPin1 22
#define trigPin1 23
*/

#define led 13

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(trigPin0, OUTPUT);
  pinMode(echoPin0, INPUT);

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
    pinMode(led, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  printUs0();
  printUs1();
    delay(DELAY);
}


void printUs1() {
  long duration, distance;
  digitalWrite(trigPin1, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin1, HIGH);
  //delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin1, LOW);
  duration = pulseIn(echoPin1, HIGH);
  distance = (duration / 2) / 29.1;
  if (distance < 4) {  // This is where the LED On/Off happens
    digitalWrite(led, HIGH); // When the Red condition is met, the Green LED should turn off

  }
  else {
    digitalWrite(led, LOW);

  }
  if (distance >= 200 || distance <= 0) {
    Serial.print("us 1:\t");
    Serial.println("Out of range");
  }
  else {
    Serial.print("Distance from us 1: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
  //Serial.println(duration);
}

void printUs0() {
  long duration, distance;
  digitalWrite(trigPin0, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin0, HIGH);
  //delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin0, LOW);
  duration = pulseIn(echoPin0, HIGH);
  distance = (duration / 2) / 29.1;
  if (distance < 4) {  // This is where the LED On/Off happens
    digitalWrite(led, HIGH); // When the Red condition is met, the Green LED should turn off

  }
  else {
    digitalWrite(led, LOW);

  }
  if (distance >= 200 || distance <= 0) {
    Serial.print("us 0:\t");
    Serial.println("Out of range");
  }
  else {
    Serial.print("Distance from us0: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
  //Serial.println(duration);
}
