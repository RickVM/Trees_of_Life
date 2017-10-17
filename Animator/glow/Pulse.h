#pragma once

#include <Arduino.h>
#include <FastLED.h>

const int bigWaveBrightness[] = {5, 5, 10, 15, 20, 25, 30, 35, 40, 45, 52, 59, 68, 76, 85, 95, 100, 95, 60, 40, 30, 25, 20, 19, 18, 17, 16, 15, 14, 13, 10};
const int smallWaveBrightness[] = {5, 5, 10, 15, 20, 25, 30, 35, 40, 45, 52, 59, 68, 76, 85, 95, 100, 95, 60, 40, 30, 25, 20, 19, 18, 17, 16, 15, 14, 13, 10};

class Pulse {
  private:
    int NUM_LEDS;
    int hue;
    //Heartbeat pulse vars.
    //Used for old pulse 'tick' (line)
    const int hPulseSize = 5; //This number cannot be lower than pulseSpeed, otherwise the pulse will skip leds!
    const int pulseSpeed = 3; //2 //Increasing this number makes the pulse go faster.
    const double fallSpeed = 3;
    //Used for new pulse (wavelike)
    const int firstWave = 40;
    const int secondWave =  40;
    const int waveTail = 20; //2/3 head, 1/3 tail
    const int firstWaveAmplitudeFactor = 2;
    const int secondWaveAmplitudeFactor = 1.3;
    //Used for custom wave
    const int waveSize = 30;
    const int brightness = 200;
    CRGB* leds;
    int pulseIndex;
    double pulseDec; //This allows us to more precisely control the fallspeed.

    void SawToothWave(int x, int waveSize, int tailWave, int amplitudeFactor);
  public:
    Pulse(CRGB* StripLeds, int Num_leds, int Hue);
    bool tick();
    bool tickCustomWave();
    bool tickSawWave();
    bool fall();
};
