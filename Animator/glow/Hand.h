#pragma once

#include <LinkedList.h>
#include "Pulse.h"
#include "Ledstrips.h"
#include "AnimationSettings.h"

enum handState {
  Rest,
  Pulsing,
};

class Hand {
  private:
    ledstrip** strips;
    int nrOfStrips;
    LinkedList<Pulse *> Pulses;
    LinkedList<Pulse *> RestPulses;
    handState currentState;
    long lastRestPulseTime;
    void makePulse(int strip, double intensity); //Makes a new pulse for the strip
    void deletePulse(int i);//Deletes a pulse at the given index from the pulses list
    void deleteRestPulse(int i); //Deletes a pulse at the given index from the pulses list
    void makeRestPulse(int strip);
    void pulseFade(); //Fades active strips
    void doPulse(); //Does a tick for all regular pulses and deletes finished ones.
    void doRestPulse();  //Does a tick for all test pulses and deletes finished ones.

  public:
    Hand(ledstrip** Strips, int NrOfStrips);
    void setState(handState state);
    void executeState();
    void fakeRestPulse(); //For test purposes
    void collapsePulse();//Collapse a pulse backwards
    void makePulses(double intensity); //Makes a pulse for all strips in the hand
    void deletePulses() ; //Delete all pulses

    //Triggers makepulse every pulseTime seconds
    //Used for automatic testing purposes
    void doTestPulses();
};

