#include "Pulse.h"


Pulse::Pulse(CRGB * StripLeds, int Num_leds, int Hue) {
  this->leds = StripLeds;
  this->NUM_LEDS = Num_leds;
  this->hue = Hue;
  pulseIndex = 0 - (firstWave + secondWave); // in case of tickSawWave
  //PulseeIndex = 0 - (waveSize * 2) in case of customwave;
}

bool Pulse::tick() {
  if ((pulseIndex + hPulseSize ) >= NUM_LEDS) {
  }
  else {
    for ( int i = 0; i < hPulseSize; i ++) {
      leds[pulseIndex + i] = CHSV(hue, 255, brightness);
      //leds[pulseIndex + i] = currentColour[0];
    }
    pulseIndex += pulseSpeed;
    pulseDec = pulseIndex; //Inefficient to do this, but performance is good enough.
  }
  return true;
}

void Pulse::SawToothWave(int x, int waveSize, int tailWave, int amplitudeFactor) {
  int currentPixel;
  int firstWaveSize = waveSize - tailWave;
  for (currentPixel = 0; currentPixel < firstWaveSize; currentPixel++) {
    if ((currentPixel + x) >= 0 && (currentPixel + x ) < NUM_LEDS) {
      int waveParticle = (triwave8(currentPixel) * amplitudeFactor + 20);

      if (waveParticle > 0) {
        leds[x + currentPixel] = CHSV(hue, 255, waveParticle);
      }
    }
  }
  //Serial.println("Secondwave");
  for (int i = tailWave, e = firstWaveSize; i >= 0; i--, currentPixel++, e -= 2) {
    if ((currentPixel + x) >= 0 && (currentPixel + x ) < NUM_LEDS) {
      int waveParticle = (triwave8(e) * amplitudeFactor + 20);
      if (waveParticle > 0) {
        leds[x + currentPixel] = CHSV(hue, 255, waveParticle);
      }
    }
  }
}

bool Pulse::tickCustomWave() {
  if (pulseIndex + (waveSize * 2) >= NUM_LEDS) {
    //Do nothing
  }
  else {
    int e = 0;
    Serial.println("Bigwave");
    for (int i = 0; i < waveSize; i++, e++) {
      Serial.print("LED:");
      Serial.print(pulseIndex + i);
      Serial.print("\t");
      Serial.println(bigWaveBrightness[i]);
      leds[pulseIndex + i] = CHSV(hue, 255, bigWaveBrightness[i]);
    }
    Serial.println("Smallwave");
    for (int i = 0; i < waveSize; i++, e++) {
      Serial.print("LED:");
      Serial.print(pulseIndex + e);
      Serial.print("\t");
      Serial.println(bigWaveBrightness[i]);
      leds[pulseIndex + e] = CHSV(hue, 255, smallWaveBrightness[i]);
    }
    pulseIndex += pulseSpeed;
    pulseDec = pulseIndex; //THIS ENHANCES CUSTOMIZABILITY BUT MIGHT AFFECT SPEED. VERIFY LATER!
  }
  return true;
}
bool Pulse::tickSawWave() {
  if (pulseIndex >= NUM_LEDS) {
    return false;
  }
  else {
    SawToothWave(pulseIndex + secondWave, firstWave, waveTail, firstWaveAmplitudeFactor);
    SawToothWave(pulseIndex, secondWave, waveTail, secondWaveAmplitudeFactor);
    //sineWave(pulseIndex+waveSize+25, waveSize, 5);
    pulseIndex += pulseSpeed;
    pulseDec = pulseIndex; //THIS ENHANCES CUSTOMIZABILITY BUT MIGHT AFFECT SPEED. VERIFY LATER!
  }
  return true;
}
bool Pulse::fall() {
  //Serial.println(pulseIndex <= (pulseIndex - (firstWave + secondWave + 1));
  if (pulseIndex <= (0 - (firstWave + secondWave))) { //Customized for sawtoothwave size!!
    return false; //finished collapsing
  }
  else {
    SawToothWave(pulseIndex + secondWave, firstWave, waveTail, firstWaveAmplitudeFactor);
    SawToothWave(pulseIndex, secondWave, waveTail, secondWaveAmplitudeFactor);
  }
  pulseIndex -= fallSpeed;
  return true;
}
