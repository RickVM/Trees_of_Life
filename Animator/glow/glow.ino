/*V2.1 Demo version. This program controls 2 LED-Strips based upon given commands through serial.
   LED strip is assumed to have 420 LEDS spread over 7 meters. (60/M)
   Author: Rick van Melis & Simon Lit
   For jack connectors of octoboard see: https://www.pjrc.com/store/octo28_adaptor.html

  Fastled library is used to control the leds. (google it)
  The following linked list library is used to keep a list of pulses https://github.com/ivanseidel/LinkedList

  Testing strips that are used are: ws2812 RGB strips.
  The final strips that are used are: ws2811 GRB strips. (current prototype strip is 40 leds big);
  2 types of final strips:
  wS2811 which is GRB
  wS2811s is BRG
*/


#include <LinkedList.h>
#include "FastLED.h"
#include "UART.h"
#include "I2C.h"
#include "Pulse.h"

FASTLED_USING_NAMESPACE

#define ID 3

//Pinout
//Brown and blue
#define DATA1_PIN 2
#define DATA2_PIN 14
#define DATA3_PIN 7
#define DATA4_PIN 8

#define DATA5_PIN 6
#define DATA6_PIN 20
#define DATA7_PIN 21
#define DATA8_PIN 5

//Controller-Strip types
#define NUM_STRIPS 4 //When adjusting this, remember to also comment/uncomment FastLED.addleds in setup, and the ledstrip* array!!
#define NUM_LEDS_PER_STRIP 100
#define NUM_LEDS NUM_LEDS_PER_STRIP*2 //2 strips high
#define BRIGHTNESS  125 //200
#define FRAMES_PER_SECOND  40
#define FADER 40
#define PULSEFADER 40
#define FALL_FADER 30
#define FPS 40

//Program assumes all used led strips to contain the same properties as listed below.


#define LED 13

struct ledstrip {
  CRGB leds[(NUM_LEDS)];
} strip1, strip2, strip3, strip4; //Change this when adjusting the nr of ledstrips!
ledstrip* strips[] = {&strip1, &strip2, &strip3, &strip4}; //This also!

//Communication
#define BAUD_RATE 57600
#define COMMUNICATION_METHODE 1//1 for serial, 2 for I2C
Communication* COM;

unsigned long desyncTime = 8000;
//Rest pulse vars
//Pulse vars
const int pulseHue = 225;
const int restPulseHue = 90;
long lastRestPulseTime;
const long RestPulseTime = 2500;

double pulse5Intensity = 1;
double pulse6Intensity = 0.7;
double pulse7Intensity = 0.5;
double pulse8Intensity = 0.3;
double pulse9Intensity = 0.25;
double pulse10Intensity = 0.2;
double pulse11Intensity = 0.1;


enum currentStates {
  Rest,
  Pulsing,
  Synchronized,
  DeSync
};
currentStates currentState;

long lastUpdate = 0;

//Beat Flash vars
const int nrOfBeats = 4;
const int nrOfFlashes = 1;
const int beatFrames = 10; //Number of frames used in the brightness transition
const int beatDelay = 18; //Delay between each frame
const int beatBrightness = 100; // Total brightness the flash reaches.
const int flashSpeed = 10;
const int flashR = 255;
const int flashG = 0;
const int flashB = 0;
const int smallValveDelay = 10;
const int bigValveDelay = 450;

LinkedList<Pulse *> Pulses;
LinkedList<Pulse *> RestPulses;

void executeState() {
  //Serial.print("Executing state: \t");
  switch (currentState) {
    case Rest:
      //Serial.println("Rest");
      pulseFade();
      fakeRestPulse(); //For test purposes
      doRestPulse();
      FastLED.show();
      break;
    case Pulsing:
      //Serial.println("Pulsing");
      if (Pulses.size() == 0) {
        currentState = Rest;
        return;
      }
      pulseFade();
      doPulse();     //fillStripWithColor();
      FastLED.show();
      break;
    case Synchronized:
      //Serial.println("Synced");
      //flash state (flash)
      valveBeat();
      currentState = Pulsing;
      break;

    case DeSync:
      //Serial.println("Desync");
      unsigned long startTime = millis();
      while ((desyncTime + startTime)  > millis()) {
        for (int i = 0; i < NUM_STRIPS; i++) {
          fadeToBlackBy(strips[i]->leds, NUM_LEDS, FALL_FADER);
        }
        collapsePulse(); //Gets all existing pulses from a list and collapses them.
        FastLED.show();
      }
      currentState = Pulsing;
      break;
  }
}

void mPulse(double intensity) {
  if (currentState == Rest) {
    currentState = Pulsing;
  }
  if (currentState == Pulsing) { //Only if in pulse or rest state. When moving this to another file, dont forget to make currentState extern!
    //MAKE 1 PULSE FOR EACH STRIP
    for (int i = 0; i < NUM_STRIPS; i++) {
      makePulse(i, intensity);
    }
  }
}

void readInput() {
  COMMANDS x = COM->readCommand(ID);
  switch (x) {
    case error:
      //Do nothing
      break;
    case pulse5:
      mPulse(pulse5Intensity);
      break;
    case pulse6:
      mPulse(pulse6Intensity);
      break;
    case pulse7:
      mPulse(pulse7Intensity);
      break;
    case pulse8:
      mPulse(pulse8Intensity);
      break;
    case pulse9:
      mPulse(pulse9Intensity);
      break;
    case pulse10:
      mPulse(pulse10Intensity);
      break;
    case pulse11:
      mPulse(pulse11Intensity);
      break;
    case flash:
      deletePulses();
      currentState = Synchronized;
      break;
    case backward:
      currentState = DeSync;
      break;
    case rest:
      //do nothing
      break;
  }
}


void printStartupDebug() {
  Serial.print("ID:\t");
  Serial.println(ID);
}

void setup() {
  //delay(3000); // 3 second delay for recovery
  //wS2811 which is GRB
  //wS2811s is BRG
  //Couple 1
  FastLED.addLeds<WS2811, DATA1_PIN, GRB>(strips[0]->leds, 0, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2811, DATA2_PIN, GRB>(strips[0]->leds, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);
  //Couple 2

  FastLED.addLeds<WS2811, DATA3_PIN, GRB>(strips[1]->leds, 0,  NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2811, DATA4_PIN, GRB>(strips[1]->leds, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);

  //Couple 3
  FastLED.addLeds<WS2811, DATA5_PIN, GRB>(strips[2]->leds, 0,  NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2811, DATA6_PIN, GRB>(strips[2]->leds, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);

  //Couple 4
  FastLED.addLeds<WS2811, DATA7_PIN, GRB>(strips[3]->leds, 0, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2811, DATA8_PIN, GRB>(strips[3]->leds, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);

  //Set random seed
  randomSeed(analogRead(0));
  //FastLED.setBrightness(BRIGHTNESS);
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  Pulses = LinkedList<Pulse*>();
  RestPulses = LinkedList<Pulse*>();

  resetStrips();
  FastLED.show();

  //Communication
  switch (COMMUNICATION_METHODE) {
    case 1:
      COM = new UART(BAUD_RATE);
      break;
    case 2:
      COM = new I2C(ID);
      break;
  };
  COM->Begin();
  currentState = Pulsing;
  lastRestPulseTime = 0;
  //fillStripWithColorTemp();
  delay(500);
  printStartupDebug();
  FastLED.show();
  delay(2500);
}

void loop() {
  //currentState = Synchronized;
  //doTestPulses();
  //currentState = Pulsing;

  readInput();
  long currentTime = millis();
  if (currentTime >= (lastUpdate + (1000 / FPS))) {
    executeState();
    lastUpdate = millis();
  }
};
