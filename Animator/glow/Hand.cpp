#include "Hand.h"

Hand::Hand(ledstrip** Strips, int NrOfStrips)
{
  strips = Strips;
  NrOfStrips = nrOfStrips;
  Pulses = LinkedList<Pulse*>();
  RestPulses = LinkedList<Pulse*>();
  currentState = Rest;
  lastRestPulseTime = 0;
  Serial.println("Constructed a hand");
}

void Hand::makePulses(double intensity) {
  if (currentState == Rest) {
    currentState = Pulsing;
  }
  if (currentState == Pulsing) { //Only if in pulse or rest state. When moving this to another file, dont forget to make currentState extern!
    //MAKE 1 PULSE FOR EACH STRIP
    for (int i = 0; i < nrOfStrips; i++) {
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
  for (int i = 0; i < Pulses.size(); i++) {
    //Serial.println("Doing a pulse in dopulse");
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
  //Serial.println("Pulse made");
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


//Triggers makepulse every pulseTime seconds
//Used for automatic testing purposes
void Hand::doTestPulses() {
  long Time = millis();
  //int strip = random(0, 1);
  if ((lastRestPulseTime + 2500 ) < Time)
  {
    Serial.println("Making test pulse");
    for (int i = 0; i < nrOfStrips; i++) {
      makePulse(i, 1);
    }
    lastRestPulseTime = Time;
  }
}

void Hand::executeState() {
  //Serial.print("Executing state: \t");
  switch (currentState) {
    case Rest:
      //Serial.println("Rest");
      pulseFade();
      fakeRestPulse(); //For test purposes
      doRestPulse();
      break;
    case Pulsing:
      //Serial.println("Pulsing");
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
  currentState = state;
}

void Hand::pulseFade() {
  //Serial.println("FADING STRIPS IN PULSE/RESTPULSE MODE");
  for (int i = 0; i < nrOfStrips; i++) {
    fadeToBlackBy(strips[i]->leds, strips[i]->nrOfLeds, FADER);
  }
}
