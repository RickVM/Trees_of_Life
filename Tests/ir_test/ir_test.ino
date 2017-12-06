#include <DistanceGP2Y0A21YK.h>


#define echoPin 11
#define trigPin 12

#define led 13

DistanceGP2Y0A21YK Dist;
int distance;

const int setDistance = 15;
const int delayTime = 100;

#define sensorPin A7

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Dist.begin(sensorPin);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(led, OUTPUT);

  Serial.println("Boot complete");
}

void loop() {
  // put your main code here, to run repeatedly:
  //printIr();
  printUs();
  delay(delayTime);
}

void printIr() {
  distance = Dist.getDistanceCentimeter();
  double val = 0;
  int i = analogRead(sensorPin);
  val = (6762 / (i - 9)) - 4;

  Serial.print("\nDistance in: ");
  Serial.print("\t Voltage:");
  Serial.print(i);
  Serial.print("\t formula: ");
  Serial.print(val);
  Serial.print("\t library: ");
  Serial.println(distance);
}


void printUs() {
  long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  //  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;


  //Toggle led
  if (distance < setDistance) {  // This is where the LED On/Off happens
    digitalWrite(led, HIGH); // When the Red condition is met, the Green LED should turn off

  }
  else {
    digitalWrite(led, LOW);

  }
  if (distance >= 200 || distance <= 0) {
    Serial.println("Out of range");
    return;
  }
  else {
    Serial.print("Distance from us: ");
    Serial.print(distance);
    Serial.println(" cm");
  }

  //Serial.println(duration);
}
