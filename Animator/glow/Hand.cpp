#include "Hand.h"

Hand::Hand(ledstrip* Strip1, ledstrip* Strip2, int NrOfStrips)
{
  strips[0] = Strip1;
  strips[1] = Strip2;
  nrOfStrips = NrOfStrips;
  Pulses = LinkedList<Pulse*>();
  RestPulses = LinkedList<Pulse*>();
  Serial.println(currentState);
  currentState = Rest;
  Serial.println(currentState);
  lastRestPulseTime = 0;
  Serial.println("Constructed a hand");
}

void Hand::makePulses(double intensity) {
  Serial.println("Makepulses");
  Serial.print("State before doing something:\t");
  Serial.println(this->currentState);
  if (this->currentState == Rest) {
    Serial.println("Detecting rest state with a pulse; changing to pulse state");
    currentState = Pulsing;
    Serial.println("State Changed");
  }
  if (this->currentState == Pulsing) { //Only if in pulse or rest state. When moving this to another file, dont forget to make currentState extern!
    //MAKE 1 PULSE FOR EACH STRIP
    Serial.print("Nr of strips:\t");
    Serial.println(nrOfStrips);
    for (int i = 0; i < nrOfStrips; i++) {
      Serial.println("Going to make a pulse");
      makePulse(i, intensity);
    }
  }
}

void Hand::fakeRestPulse() { //For test purposes
  long currentTime = millis();
  Serial.println("Checking if we should fake");
  if (currentTime >= (lastRestPulseTime + RestPulseTime)) {
    Serial.println("Faking a rest pulse");
    lastRestPulseTime = currentTime;
    int strip = random(0, nrOfStrips);
    makeRestPulse(strip);
  }
}

void Hand::doRestPulse() {
  for (int i = 0; i < RestPulses.size(); i++) {
    Serial.println("Doing a rest pulse");
    if (RestPulses.get(i)->tickRestWave()) {
    }
    else {
      deleteRestPulse(i);
    }
  }
}

//Makes a new pulse for the strip
void Hand::makeRestPulse(int strip) {
  Serial.println("Making a rest pulse");
  int IndexNumber = random(0, strips[strip]->nrOfLeds);
  Pulse * P = new Pulse(strips[strip]->leds, strips[strip]->nrOfLeds, restPulseHue, IndexNumber);
  RestPulses.add(P);
}

//Deletes a pulse at the given index from the pulses list
void Hand::deleteRestPulse(int i) {
  Serial.println("Deleting rest-pulse");
  Pulse * P = RestPulses.remove(i);
  delete(P);
  P = NULL;
}

//Does a tick for all pulses and deletes finished ones.
void Hand::doPulse() {
  Serial.println("Entered dopulse");
  for (int i = 0; i < Pulses.size(); i++) {
      Serial.println("Doing a pulse tick");
    if (Pulses.get(i)->tickSawWave()) { //tickSawWave
    }
    else {
      deletePulse(i);
    }
  }
}

//Collapse a pulse backwards
//Used for 'desync'
void Hand::collapsePulse() {
  for (int i = 0; i < Pulses.size(); i++) {
    Serial.println("Collapsing a pulse");
    if (Pulses.get(i)->fall()) {
    }
    else {
      Serial.println("Deleting a pulse");
      deletePulse(i);
    }
  }
}

//Makes a new pulse for the strip
void Hand::makePulse(int strip, double intensity) {
  Serial.printf("Making a pulse with intensity: %f for: ", intensity);
  Serial.println(strip);
  Pulse * P = new Pulse(strips[strip]->leds, strips[strip]->nrOfLeds, pulseHue, intensity);
  Pulses.add(P);
  Serial.println("Pulse made");
}

void Hand::deletePulses() {
  int Size = Pulses.size();
  Serial.print("Pulses size:  ");
  Serial.println(Size);
  for (int i = 0; i < Size; i++) {
    deletePulse(0);
  }
}

//Deletes a pulse at the given index from the pulses list
void Hand::deletePulse(int i) {
  Serial.println("Deleting pulse");
  Pulse * P = Pulses.remove(i);
  delete(P);
  P = NULL;
}

void Hand::executeState() {
  Serial.print("Executing hand-state: \t");
  Serial.print(currentState);
  switch (currentState) {
    case Rest:
      Serial.println("Rest");
      pulseFade();
      fakeRestPulse(); //For test purposes
      doRestPulse();
      break;
    case Pulsing:
      Serial.println("Pulsing");
      if (Pulses.size() == 0) {
        currentState = Rest;
        return;
      }
      pulseFade();
      doPulse();
      break;
  }
}

void Hand::setState(handState state) {
  Serial.print("Setting the state with: " );
  Serial.println(state);
  currentState = state;
}

void Hand::pulseFade() {
  //Serial.println("FADING STRIPS IN PULSE/RESTPULSE MODE");
  for (int i = 0; i < nrOfStrips; i++) {
    fadeToBlackBy(strips[i]->leds, strips[i]->nrOfLeds, FADER);
  }
}

void Hand::Test() {
  Serial.println("This is a test");
  Serial.print("State:\t");
  Serial.println(currentState);
}
