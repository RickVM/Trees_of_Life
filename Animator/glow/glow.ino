/*V2.1 Demo version. This program controls 2 LED-Strips based upon given commands through serial.
   LED strip is assumed to have 420 LEDS spread over 7 meters. (60/M)
   Author: Rick van Melis & Simon Lit
   For jack connectors of octoboard see: https://www.pjrc.com/store/octo28_adaptor.html

  Fastled library is used to control the leds. (google it)
  The following linked list library is used to keep a list of pulses https://github.com/ivanseidel/LinkedList
*/

#include <LinkedList.h>
#include "FastLED.h"
#include "Communication.h"

//Program assumes all used led strips to contain the same properties as listed below.
FASTLED_USING_NAMESPACE
#define LED_TYPE    WS2812
#define COLOR_ORDER RGB
#define NUM_LEDS  600

//6m is 360, 7m is 420, 10M is 600
#define BRIGHTNESS  200 //200
#define FRAMES_PER_SECOND  1000
#define FADER 60
#define PULSEFADER 60
#define FALL_FADER 20

//Pinout
//Brown and blue
#define NUM_STRIPS 2 //When adjusting this, remember to also comment/uncomment FastLED.addleds in setup, and the ledstrip* array!!
#define DATA0_PIN 2
#define DATA1_PIN 14
#define DATA2_PIN 8
#define DATA3_PIN 7
#define LED 13

//Communication
#define BAUD_RATE 57600

struct ledstrip {
  CRGB leds[NUM_LEDS];
} strip1, strip2, strip3, strip4; //Change this when adjusting the nr of ledstrips!
ledstrip* strips[] = {&strip1, &strip2, &strip3, &strip4}; //This also!

const int stripPins[] {DATA0_PIN, DATA1_PIN, DATA2_PIN, DATA3_PIN};
//Heartbeat pulse vars.
//Used for old pulse 'tick' (line)
const int hPulseSize = 5; //This number cannot be lower than pulseSpeed, otherwise the pulse will skip leds!
const int pulseSpeed = 4; //Increasing this number makes the pulse go faster.
const int r = 17; //Pulse colors
const int g = 69;
const int b = 223;
const double fallSpeed = 4;
//Used for new pulse (wavelike)
int pulseHue = 172;
const int firstWave = 60;
const int secondWave =  60;
const int waveTail = 20; //2/3 head, 1/3 tail
const int firstWaveAmplitudeFactor = 2;
const int secondWaveAmplitudeFactor = 1.3;
//Used for custom wave
int bigWaveBrightness[] {5, 5, 10, 15, 20, 25, 30, 35, 40, 45, 52, 59, 68, 76, 85, 95, 100, 95, 60, 40, 30, 25, 20, 19, 18, 17, 16, 15, 14, 13, 10};
const int waveSize = sizeof(bigWaveBrightness) / sizeof(bigWaveBrightness[0]);
int smallWaveBrightness[waveSize];
double smallWaveModifier = 0.5;

//Beat Flash vars
const int nrOfBeats = 4;
const int nrOfFlashes = 1;
const int beatFrames = 10; //Number of frames used in the brightness transition
const int beatDelay = 10 / beatFrames; //Delay between each frame
const int beatBrightness = 180; // Total brightness the flash reaches.
const int flashSpeed = 10;
const int rFlash = 0;
const int gFlash = 0;
const int bFlash = 70;
const int smallValveDelay = 5;
const int bigValveDelay = 450;

//Communication
Communication* S1;

void cHue() {
  if (pulseHue >= 250) {
    pulseHue = 1;
  }
  pulseHue += 1;
}

class Pulse {
  private:
    ledstrip* strip;
    int pulseIndex;
    double pulseDec; //This allows us to more precisely control the fallspeed.

    void SawToothWave(int x, int waveSize, int tailWave, int amplitudeFactor) {
      int currentPixel;
      int firstWaveSize = waveSize - tailWave;
      for (currentPixel = 0; currentPixel < firstWaveSize; currentPixel++) {
        int waveParticle = (triwave8(currentPixel) * amplitudeFactor + 20);
        if (waveParticle > 0 && (x + currentPixel) < NUM_LEDS) {
          //Serial.println(waveParticle);
          if ((x + currentPixel) >= 0) {
            strip->leds[x + currentPixel] = CHSV(pulseHue, 255, waveParticle);
          }
        }
      }
      //Serial.println("Secondwave");
      for (int i = tailWave, e = firstWaveSize; i >= 0; i--, currentPixel++, e -= 2) {
        int waveParticle = (triwave8(e) * amplitudeFactor + 20);
        if (waveParticle > 0 && (x + currentPixel) < NUM_LEDS && (x + currentPixel) >= 0) {
          //Serial.println(waveParticle);
          strip->leds[x + currentPixel] = CHSV(pulseHue, 255, waveParticle);
        }
      }
    }

  public:
    Pulse(ledstrip* Strip) {
      this->strip = Strip;
      pulseIndex = 0;
    }
    bool tick() {
      if ((pulseIndex + hPulseSize ) >= NUM_LEDS) {
      }
      else {
        for ( int i = 0; i < hPulseSize; i ++) {
          strip->leds[pulseIndex + i] = CHSV(pulseHue, 255, BRIGHTNESS);
          //strip->leds[pulseIndex + i] = currentColour[0];
        }
        pulseIndex += pulseSpeed;
        pulseDec = pulseIndex; //Inefficient to do this, but performance is good enough.
      }
      return true;
    }
    bool tickCustomWave() {
      if (pulseIndex + (waveSize * 2) >= NUM_LEDS) {
        //Do nothing
      }
      else {
        int e;
        Serial.println("Bigwave");
        for (int i = 0; i < waveSize; i++, e++) {
          Serial.print("LED:");
          Serial.print(pulseIndex + i);
          Serial.print("\t");
          Serial.println(bigWaveBrightness[i]);
          strip->leds[pulseIndex + i] = CHSV(pulseHue, 255, bigWaveBrightness[i]);
        }
        Serial.println("Smallwave");
        for (int i = 0; i < waveSize; i++, e++) {
          Serial.print("LED:");
          Serial.print(pulseIndex + e);
          Serial.print("\t");
          Serial.println(bigWaveBrightness[i]);
          strip->leds[pulseIndex + e] = CHSV(pulseHue, 255, smallWaveBrightness[i]);
        }
        pulseIndex += pulseSpeed;
        pulseDec = pulseIndex; //THIS ENHANCES CUSTOMIZABILITY BUT MIGHT AFFECT SPEED. VERIFY LATER!
      }
      return true;
    }
    bool tickSawWave() {
      if ((pulseIndex + firstWave + secondWave) >= NUM_LEDS) {
        //Do nothing
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
    bool fall() {
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
};

LinkedList<Pulse *> Pulses;

void setup() {
  delay(3000); // 3 second delay for recovery
  FastLED.addLeds<LED_TYPE, DATA0_PIN, COLOR_ORDER>(strips[0]->leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  if (NUM_STRIPS > 1) {
    FastLED.addLeds<LED_TYPE, DATA1_PIN, COLOR_ORDER>(strips[1]->leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    if (NUM_STRIPS > 2) {
      FastLED.addLeds<LED_TYPE, DATA2_PIN, COLOR_ORDER>(strips[2]->leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
      if (NUM_STRIPS > 3) {
        FastLED.addLeds<LED_TYPE, DATA3_PIN, COLOR_ORDER>(strips[3]->leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
      }
    }
  }
  //FastLED.setBrightness(BRIGHTNESS);
  Serial.begin(9600);
  Serial.print("Wave size:");
  Serial.println(waveSize);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  Pulses = LinkedList<Pulse*>();
  resetStrips();
  FastLED.show();

  //Set up smallWave
  for (int i = 0; i < waveSize; i++) {
    smallWaveBrightness[i] = (bigWaveBrightness[i] * smallWaveModifier);
    Serial.print("Smallwave Index:");
    Serial.print(i);
    Serial.print("\t");
    Serial.print(smallWaveBrightness[i]);
    Serial.print("Should be:\t");
    Serial.println((bigWaveBrightness[i] * smallWaveModifier));
  }
  //Communication
  S1 = new Communication(1, BAUD_RATE);
  S1->Begin();
}

void resetStrips() {
  for (int j = 0; j < NUM_STRIPS; j++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      strips[j]->leds[i] = CRGB(0, 0, 0);
    }
  }
  FastLED.setBrightness(BRIGHTNESS);
}

void fillStripWithColor() {
  for (int i = 0; i < NUM_STRIPS; i++) {
    for (int e = 0; e < NUM_LEDS; e++) {
      strips[i]->leds[e] = CHSV(pulseHue, 255, 18);
    }
  }
}
void loop() {
  //normal pulseState (reset state)
  for (int i = 0; i <= NUM_STRIPS; i++) {
    fadeToBlackBy(strips[i]->leds, NUM_LEDS, FADER);
  }
  cHue();
  //fillStripWithColor();
  doPulse();
  //Read serial port
  COMMANDS x = S1->readSerialPort();
  switch (x) {
    case error:
      break;
    case pulse:
      //received Pulse
      //MAKE 2 PULSES SPREAD OUT
      for (int i = 0; i < NUM_STRIPS; i++) {
        makePulse(i);
      }
      break;
    case flash:
      //flash state (flash)
      deletePulses();
      valveBeat();
      break;
    case backward:
      long loopTime = 8000;
      long startTime = millis();
      while ((loopTime + startTime)  > millis()) {
        for (int i = 0; i < NUM_STRIPS; i++) {
          fadeToBlackBy(strips[i]->leds, NUM_LEDS, FALL_FADER);
        }
        collapsePulse(); //Gets all existing pulses from a list and collapses them.
        FastLED.show();
      }
      break;
      /*case reset:
        //normal pulseState (reset state)
        for (int i = 0; i < NUM_STRIPS; i++) {
          fadeToBlackBy(strips[i]->leds, NUM_LEDS, 255);
        }
        doPulse();
        break;*/

  };
  FastLED.show();

}
