#include "AnimationSettings.h"

//Preps strip for beats, beats and resets strip
void valveBeat() {
  //long T = millis();
  setColourForTreeBeat();
  for (int i = 0; i < nrOfBeats; i++) {
    beat();
    FastLED.delay(smallValveDelay);
    beat();
    FastLED.delay(bigValveDelay);
  }
  resetStrips();
  FastLED.show();
  //Serial.print("Time for flash:");
  //Serial.println((millis()-T));
}

//Sub-Function to dispay a beat
void beat() {
  Serial.println("Beating..");
  int b = 0;
  int x = beatBrightness / beatFrames;

  for (int i = 0; i < beatFrames; i++) {
    FastLED.setBrightness(b += x);
    FastLED.show();
    FastLED.delay(beatDelay);
  }
  for (int i = 0; i < (beatFrames); i++) { //Do beatFrames -1 if you want to dim leds but not turn them off
    FastLED.setBrightness(b -= x);
    FastLED.show();
    FastLED.delay(beatDelay);
  }
}

//Sub-function that sets the right color for beats.
void setColourForTreeBeat() {
  for (int i = 0; i < NUM_STRIPS_TOTAL; i++) {
    for (int j = 0; j < strips[i]->nrOfLeds; j++) {
      strips[i]->leds[j] = CRGB(flashR, flashG, flashB);
    }
  }
}

void resetStrips() {
  for (int j = 0; j < NUM_STRIPS_TOTAL; j++) {
    for (int i = 0; i < strips[i]->nrOfLeds; i++) {
      strips[j]->leds[i] = CRGB(0, 0, 0);
    }
  }
  FastLED.setBrightness(BRIGHTNESS);
}

/*  Old animations

  void twinkle(ledstrip & strip) {
  // random colored speckles that blink in and fade smoothly

  long Time = millis();
  if (Time > (strip.lastTwinkle + twinkleSpeed)) {
    strip.lastTwinkle = Time;
    int pos = random16(NUM_LEDS);
    if (pos > (NUM_LEDS - 2)) {
      pos = (NUM_LEDS - 2);
    }
    else if (pos < 1) {
      pos = 2;
    }
    strip.leds[pos].setRGB(rT, gT, bT);
    strip.leds[(pos + 1)].setRGB(rT, gT, bT);
    strip.leds[(pos - 1)].setRGB(rT, gT, bT);
  }
  }*/

