//Deletes a pulse at the given index from the pulses list
void deletePulses() {
  int Size = Pulses.size();
  for (int i = 0; i < Size; i++) {
    deletePulse(0);

  }
}

void deletePulse(int i) {
  Serial.println("Deleting pulse");
  Pulse * P = Pulses.remove(i);
  delete(P);
  P = NULL;
}

//Does a tick for all pulses and deletes finished ones.
void doPulse() {
  for (int i = 0; i < Pulses.size(); i++) {
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
  Pulse * P = new Pulse(strips[strip]);
  Pulses.add(P);
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
