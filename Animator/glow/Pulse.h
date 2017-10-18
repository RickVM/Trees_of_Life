#pragma once

#include <Arduino.h>
#include <FastLED.h>

//These should be in the class, but wont work if done so.
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
    const int brightness = 150;
    CRGB* leds;

    double pulseDec; //This allows us to more precisely control the fallspeed.

    //used in both rest and non rest pulse
    int pulseIndex;
    
    //used for rest pulse
    const int restpulseSize = 5;
    int restpulseLifeTime = 2000;
    const int restPulseSpeed = 1;
    long startTime = 0;
    const int restHue = 90;

    void SawToothWave(int x, int waveSize, int tailWave, int amplitudeFactor);

  public:
    Pulse(CRGB* StripLeds, int Num_leds, int Hue);
    Pulse(CRGB* StripLeds, int Num_leds, int Hue, int customIndex);
    bool tick();
    bool tickCustomWave();
    bool tickSawWave();
    bool tickRestWave();
    bool fall();
};
