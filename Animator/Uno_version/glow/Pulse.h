#pragma once

#include <Arduino.h>
#include <FastLED.h>

//These should be in the class, but wont work if done so.
const int bigWaveBrightness[] = {5, 5, 10, 15, 20, 25, 30, 35, 40, 45, 52, 59, 68, 76, 85, 95, 100, 95, 60, 40, 30, 25, 20, 19, 18, 17, 16, 15, 14, 13, 10};
const int smallWaveBrightness[] = {3, 3, 5, 8, 10, 13, 15, 18, 20, 23, 26, 30, 34, 38, 43, 8, 50, 48, 30, 20, 15, 13, 10, 10, 9, 9, 8, 8, 7, 7, 5 };
extern bool stripTypeNew;

//used with the new strips
#define NEW_PULSESPEED 1
#define NEW_FALLSPEED 0.4
#define NEW_FIRSTWAVEAMPLITUDEFACTOR 4
#define NEW_SECONDWAVEAMPLITUDEFACTOR 2.6
#define NEW_RESTPULSESPEED 1

//used with the old strips
#define OLD_PULSESPEED 3
#define OLD_FALLSPEED 3
#define OLD_FIRSTWAVEAMPLITUDEFACTOR 2
#define OLD_SECONDWAVEAMPLITUDEFACTOR 1.3
#define OLD_RESTPULSESPEED 1

class Pulse {
  private:
    int num_leds;
    int hue;
    //Heartbeat pulse vars.
    //Used for old pulse 'tick' (line)
    const int hPulseSize = 5; //This number cannot be lower than pulseSpeed, otherwise the pulse will skip leds!

    //Used for new pulse (wavelike)
    //These should be made const after prototyping
    int pulseSpeed;
    double fallSpeed;
    double firstWaveAmplitudeFactor;
    double secondWaveAmplitudeFactor;
    const int firstWave = 40;
    const int secondWave =  40;
    const int waveTail = 20; //2/3 head, 1/3 tail
    //Used for custom wave
    const int waveSize = 10;
    const int brightness = 150;
    CRGB* leds;

    double pulseDec; //This allows us to more precisely control the fallspeed.

    //used in both rest and non rest pulse
    int pulseIndex;

    //used for rest pulse
    const int restpulseSize = 5;
    int restpulseLifeTime = 2000;
    double restPulseSpeed;
    long startTime = 0;

    void SawToothWave(int x, int waveSize, int tailWave);

  public:
    Pulse(CRGB* StripLeds, int Num_leds, int Hue, double intensity);

    Pulse(CRGB* StripLeds, int Num_leds, int Hue, int customIndex);
    bool tick();
    bool tickCustomWave();
    bool tickSawWave();
    bool tickRestWave();
    bool fall();
};
