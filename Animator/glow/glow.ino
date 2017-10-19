/*V2.1 Demo version. This program controls 2 LED-Strips based upon given commands through serial.
   LED strip is assumed to have 420 LEDS spread over 7 meters. (60/M)
   Author: Rick van Melis & Simon Lit
   For jack connectors of octoboard see: https://www.pjrc.com/store/octo28_adaptor.html

  Fastled library is used to control the leds. (google it)
  The following linked list library is used to keep a list of pulses https://github.com/ivanseidel/LinkedList

  Testing strips that are used are: ws2812 RGB strips.
  The final strips that are used are: ws2811 GRB strips. (current prototype strip is 40 leds big);
*/

#define ID 3


//Controller-Strip types
#if ID == 3
#define NUM_LEDS 200
#define BRIGHTNESS  125 //200
#define FRAMES_PER_SECOND  1000
#define FADER 40
#define PULSEFADER 40
#define FALL_FADER 12
#define UPDATEDELAY 20
#else

#define NUM_LEDS 600
#define BRIGHTNESS  125 //200
#define FRAMES_PER_SECOND  1000
#define FADER 60
#define PULSEFADER 60
#define FALL_FADER 20
#define UPDATEDELAY 0
#endif
bool stripTypeNew; //This is used in pulse.h to determine the settings.
int newStripIds[] {3}; //Adjust this together with precompiler if!


#include <LinkedList.h>
#include "FastLED.h"
#include "UART.h"
#include "I2C.h"
#include "Pulse.h"

//Program assumes all used led strips to contain the same properties as listed below.
FASTLED_USING_NAMESPACE

//6m is 360, 7m is 420, 10M is 600

//Pinout
//Brown and blue
#define NUM_STRIPS 2 //When adjusting this, remember to also comment/uncomment FastLED.addleds in setup, and the ledstrip* array!!
#define DATA0_PIN 2
#define DATA1_PIN 14
#define LED 13

//Communication
#define BAUD_RATE 57600
#define COMMUNICATION_METHODE 2//1 for serial, 2 for I2C

//Rest pulse vars
const int restPulseHue = HUE_RED;
long lastRestPulseTime;
const long RestPulseTime = 500;

struct ledstrip {
  CRGB leds[NUM_LEDS];
} strip1, strip2; //Change this when adjusting the nr of ledstrips!
ledstrip* strips[] = {&strip1, &strip2}; //This also!

enum currentStates {
  Rest,
  Pulsing,
  Synchronized,
  DeSync
};
currentStates currentState;

const int stripPins[] {DATA0_PIN, DATA1_PIN};

long lastUpdate = 0;


//Pulse vars
const int pulseHue = HUE_RED;

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
Communication* COM;

LinkedList<Pulse *> Pulses;
LinkedList<Pulse *> RestPulses;

void executeState() {
  Serial.print("Executing state: \t");
  switch (currentState) {
    case Rest:
      Serial.println("Rest");
      pulseFade();
      fakeRestPulse(); //For test purposes
      doRestPulse();
      FastLED.show();
      break;
    case Pulsing:
      Serial.println("Pulsing");
      if (Pulses.size() == 0) {
        currentState = Rest;
        return;
      }
      pulseFade();
      doPulse();     //fillStripWithColor();
      FastLED.show();
      break;
    case Synchronized:
      Serial.println("Synced");
      //flash state (flash)
      valveBeat();
      currentState = Pulsing;
      break;

    case DeSync:
      Serial.println("Desync");
      unsigned long loopTime = 8000;
      unsigned long startTime = millis();
      while ((loopTime + startTime)  > millis()) {
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

void readInput() {
  COMMANDS x = COM->readCommand(ID);
  switch (x) {
    case error:
      //Do nothing
      break;
    case pulse1:
      if (currentState == Rest) {
        currentState = Pulsing;
      }
      if (currentState == Pulsing) { //Only if in pulse or rest state. When moving this to another file, dont forget to make currentState extern!
        //MAKE 1 PULSE FOR EACH STRIP
        for (int i = 0; i < NUM_STRIPS; i++) {
          makePulse(i);
        }
      }
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

void checkForStripType() {
  stripTypeNew = false;
  // for (int i = 0; i < sizeof(newStripIds); i++) {
  if (ID == 3) {
    stripTypeNew = true;
    //  }
  }
}

void printStartupDebug() {
  Serial.print("ID:\t");
  Serial.println(ID);
  Serial.print("Strip type:\t");
  if (stripTypeNew) {
    Serial.println("WS2811(New)");
  }
  else {
    Serial.println("WS2812(Old)");
  }
}
void setup() {
  //delay(3000); // 3 second delay for recovery

  checkForStripType();
  if (stripTypeNew) {

    FastLED.addLeds<WS2811, DATA0_PIN, GRB>(strips[0]->leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<WS2811, DATA1_PIN, GRB>(strips[1]->leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  }
  else {

    FastLED.addLeds<WS2812, DATA0_PIN, RGB>(strips[0]->leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<WS2812, DATA1_PIN, RGB>(strips[1]->leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  }
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
  //cHue();
  readInput();

  long currentTime = millis();
  if (currentTime >= (lastUpdate + UPDATEDELAY)) {
    executeState();
    lastUpdate = millis();
  }
};

