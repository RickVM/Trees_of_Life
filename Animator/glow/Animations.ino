void treeBeat() {
  setColourForTreeBeat();
  int b = 0;
  for (int j = 0; j < nrOfBeats; j++) {
    for (int i = 0; i < 5; i++) {
      FastLED.setBrightness(b += 40);
      FastLED.show();
      FastLED.delay(1);
    }
    for (int i = 0; i < 5; i++) {
      FastLED.setBrightness(b -= 40);
      FastLED.show();
      FastLED.delay(1);
    }
  }
  resetStrips();
}

void fillStrip() {
  for (int j = 0; j < NUM_STRIPS; j++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      strips[j]->leds[i] += CRGB(1, 1, 1);
    }
  }
}

void valveBeatWave(int waveSize, int tailWave, int amplitudeFactor)
{
  //Set all leds to desired color
  for (int l = 0; l < NUM_LEDS; l++) {
    strips[0]->leds[l] = CRGB(rFlash, gFlash, bFlash);
  }
  int beatDelay = 2;
  int currentPixel;
  int firstWaveSize = waveSize - tailWave;
  for (currentPixel = 0; currentPixel < firstWaveSize; currentPixel++) {
    int waveParticle = (triwave8(currentPixel) * amplitudeFactor + 20);
    if (waveParticle > 0 && waveParticle < 255) {
      Serial.println(waveParticle);
      FastLED.setBrightness(waveParticle);
      FastLED.show();
    }
  }

  //Serial.println("Secondwave");
  for (int i = tailWave, e = firstWaveSize; i >= 0; i--, currentPixel++, e -= 2) {
    int waveParticle = (triwave8(e) * amplitudeFactor + 20);
    if (waveParticle > 0 && waveParticle < 255) {
      Serial.println(waveParticle);
        FastLED.setBrightness(waveParticle);
        FastLED.show();
      }
    }
    
  }

void valveBeat() {
  long T = millis();
  setColourForTreeBeat();
  for (int i = 0; i < nrOfBeats; i++) {
    beat();
    FastLED.delay(smallValveDelay);
    beat();
    FastLED.delay(bigValveDelay);
  }
  resetStrips();
  //Serial.print("Time for flash:");
  //Serial.println((millis()-T));
}

void beat() {
  Serial.println("Beating..");
  int b = 0;
  int x = beatBrightness/beatFrames;

  for (int i = 0; i < beatFrames; i++) {
    FastLED.setBrightness(b += x);
    FastLED.show();
    FastLED.delay(beatDelay);
  }
  for (int i = 0; i < (beatFrames-1); i++) {
    FastLED.setBrightness(b -= x);
    FastLED.show();
    FastLED.delay(beatDelay);
  }
  //Now do half so we dont turn the leds off but dim them
  FastLED.setBrightness(b -= (x/2));
  FastLED.show();
  FastLED.delay(beatDelay);
}


void resetColourForTreeBeat() {
  for (int i = 0; i < NUM_STRIPS; i++) {
    for (int j = 0; j < NUM_LEDS; j++) {
      strips[i]->leds[j] = CRGB(rFlash, gFlash, bFlash);
    }
  }
}

void setColourForTreeBeat() {
  for (int i = 0; i < NUM_STRIPS; i++) {
    for (int j = 0; j < NUM_LEDS; j++) {
      strips[i]->leds[j] = CRGB(rFlash, gFlash, bFlash);
    }
  }
}
/*
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

