void fakeRestPulse() { //For test purposes
  long currentTime = millis();
  if (lastRandomRestPulseTime + randomRestPulseTime >= currentTime) {
    lastRandomRestPulseTime = currentTime;
    int strip = random(0, 1);
    makeRestPulse(strip);
  }
}

void doRestPulse() {
  for (int i = 0; i < RestPulses.size(); i++) {
    //if (restPulses.get(i)->
  }
}

//Makes a new pulse for the strip
void makeRestPulse(int strip) {
  Serial.println("Making a rest pulse");
  Pulse * P = new Pulse(strips[strip]->leds, NUM_LEDS, restPulseHue);
  RestPulses.add(P);
}

//Deletes a pulse at the given index from the pulses list
void deleteRestPulse(int i) {
  Serial.println("Deleting rest-pulse");
  Pulse * P = RestPulses.remove(i);
  delete(P);
  P = NULL;
}

//Does a tick for all pulses and deletes finished ones.
void doPulse() {
  for (int i = 0; i < Pulses.size(); i++) {
    Serial.println("Doing a pulse in dopulse");
    if (Pulses.get(i)->tickSawWave()) { //tickSawWave
    }
    else {
      deletePulse(i);
    }
  }
}

//Collapse a pulse backwards
//Used for 'desync'
void collapsePulse() {
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
void makePulse(int strip) {
  Serial.print("Making a normal pulse\t-\t");
  Pulse * P = new Pulse(strips[strip]->leds, NUM_LEDS, pulseHue);
  Pulses.add(P);
  Serial.println("Pulse made");
}

void deletePulses() {
  int Size = Pulses.size();
  for (int i = 0; i < Size; i++) {
    deletePulse(0);

  }
}

//Deletes a pulse at the given index from the pulses list
void deletePulse(int i) {
  Serial.println("Deleting pulse");
  Pulse * P = Pulses.remove(i);
  delete(P);
  P = NULL;
}

/*
  //Triggers makepulse every pulseTime seconds
  //Used for automatic testing purposes
  void generatePulse() {
  int teststrip = 0;
  long Time = millis();
  if ((strips[teststrip]->lastPulse + pulseTime ) < Time)
  {
    makePulse(teststrip);
    strips[teststrip]->lastPulse = Time;
  }
  }*/
