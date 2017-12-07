#pragma once

#include <LinkedList.h>
#include "Pulse.h"
#include "PinoutSettings.h"
#include "AnimationSettings.h"

enum handState {
  Rest,
  Pulsing
};


class Hand {
  private:
    handState currentState;
    ledstrip* strips[2];
    int nrOfStrips;
    LinkedList<Pulse *> Pulses;
    LinkedList<Pulse *> RestPulses;
    long lastRestPulseTime;

    void deletePulse(int i);//Deletes a pulse at the given index from the pulses list
    void deleteRestPulse(int i); //Deletes a pulse at the given index from the pulses list
    void makeRestPulse(int strip); //Makes a new RestPulse for the strip
    void pulseFade(); //Fades active strips
    void makePulse(int strip, double intensity); //Makes a new pulse for the strip
    void doPulse(); //Does a tick for all regular pulses and deletes finished ones.
    void doRestPulse();  //Does a tick for all test pulses and deletes finished ones.

  public:

    Hand(ledstrip* Strip1, ledstrip* Strip2, int NrOfStrips);
    void setState(handState state);
    void executeState();
    void fakeRestPulse(); //For test purposes
    void collapsePulse();//Collapse a pulse backwards
    void makePulses(double intensity); //Makes a pulse for all strips in the hand
    void deletePulses() ; //Delete all pulses
    void Test();



};

