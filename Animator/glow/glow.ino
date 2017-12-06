/*
      Author: Rick van Melis & Simon Lit
*/

#include "FastLED.h"
#include "Hand.h"
#include "UART.h"
#include "I2C.h"
#include "AnimationSettings.h"
#include "PinoutSettings.h"


FASTLED_USING_NAMESPACE

#define ID 3
#define LED 13


//Communication
#define BAUD_RATE 57600
#define COMMUNICATION_METHODE 1//1 for serial, 2 for I2C
Communication* COM;

enum globalState {
  Normal,
  Synchronized,
  Desynchronized
} GlobalState;



Hand* hand1;
Hand* hand2;
Hand* hands[2];
Hand* activeHand = NULL;

ledstrip Strip1;
ledstrip Strip2;
ledstrip Strip3;
ledstrip Strip4; //Change this when adjusting the nr of ledstrips!
ledstrip* strips[] = {&Strip1, &Strip2, &Strip3, &Strip4}; //This also!

long randomPulseTime = 2500;

void setupHands() {
  //constructor: Hand(ledstrip* Strips[], int NrOfStrips);
  hand1 = new Hand(&Strip1, &Strip2, NUM_STRIPS_PER_HAND);
  hand2 = new Hand(&Strip3, &Strip4, NUM_STRIPS_PER_HAND);
  hands[0] = hand1;
  hands[1] = hand2;
  Serial.println("Finished setting up hands");
}

long lastUpdate = 0;
long lastTestPulseTime = 0;
long lastFlash = 0;


void executeState() {
  Serial.print("Executing state:\t");
  switch (GlobalState) {
    case Normal:
      Serial.println("Normal");
      for (int i = 0; i < NUM_HANDS; i++) {
        hands[i]->executeState();
      }
      FastLED.show();
      break;
    case Synchronized:
      Serial.println("Synchronized");
      for (int i = 0; i < NUM_HANDS; i++) {
        hands[i]->deletePulses();
      }
      valveBeat();
      for (int i = 0; i < NUM_HANDS; i++) {
        hands[i]->setState(Rest);
      }
      GlobalState = Normal;
      break;

    case Desynchronized:
      Serial.println("Desynchronized");
      unsigned long startTime = millis();
      while ((desyncTime + startTime)  > millis()) {
        for (int i = 0; i < NUM_STRIPS_TOTAL; i++) {
          fadeToBlackBy(strips[i]->leds, strips[i]->nrOfLeds, FALL_FADER);
        }
        for (int i = 0; i < NUM_HANDS; i++) {
          hands[i]->collapsePulse(); //Gets all existing pulses from a list and collapses them.
        }
        FastLED.show();
      }
      //maybe add a check here if all pulses have been collapsed; if not keep looping?
      GlobalState = Normal;
      break;
  }
}

void readInput() {
  COMMANDS x = COM->readCommand(ID);
  //Get hand, return int for wich hand
  int y = COM->getHand();//Could be 1 or 2 or 0
  if (y == 0) { //0 stands for all hands
    switch (x) {
      case error:
        //do nothing
        break;
      case flash:
        for (int i = 0; i < NUM_HANDS; i++) {
          hands[i]->deletePulses();
        }
        GlobalState = Synchronized;
        break;
      case backward:
        GlobalState = Desynchronized;
        break;
      default:
        Serial.println("WARNING: INVALID COMMAND TYPE");
        //Do nothing
        break;
    }
  }
  else {
    activeHand = hands[y - 1]; //Set the global active hand for other functions
    switch (x) {
      case error:
        //Do nothing
        break;
      case pulse5:
        activeHand->makePulses(pulse5Intensity);
        break;
      case pulse6:
        activeHand->makePulses(pulse6Intensity);
        break;
      case pulse7:
        activeHand->makePulses(pulse7Intensity);
        break;
      case pulse8:
        activeHand->makePulses(pulse8Intensity);
        break;
      case pulse9:
        activeHand->makePulses(pulse9Intensity);
        break;
      case pulse10:
        activeHand->makePulses(pulse10Intensity);
        break;
      case pulse11:
        activeHand->makePulses(pulse11Intensity);
        break;
      case rest:
        //do nothing (not implemented controller side)
        break;
      default:
        Serial.println("WARNING: INVALID COMMAND TYPE");
        //Do nothing
        break;

    }
  }
}

void setupLedStrips() {
  Serial.println("Setting up led strips");

  Strip1.nrOfLeds = NUM_LEDS_1A + NUM_LEDS_1B;
  Strip2.nrOfLeds = NUM_LEDS_2A + NUM_LEDS_2B;
  Strip3.nrOfLeds = NUM_LEDS_3A + NUM_LEDS_3B;
  Strip4.nrOfLeds = NUM_LEDS_4A + NUM_LEDS_4B;

  Strip1.leds = new CRGB[(Strip1.nrOfLeds)];
  Strip2.leds = new CRGB[(Strip2.nrOfLeds)];
  Strip3.leds = new CRGB[(Strip3.nrOfLeds)];
  Strip4.leds = new CRGB[(Strip4.nrOfLeds)];

  //wS2811 is GRB
  //wS2811s is BRG
  //Couple 1

  //Hand1
  FastLED.addLeds<WS2811, DATA1_PIN, HAND11ATYPE>(strips[0]->leds, 0, NUM_LEDS_1A).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2811, DATA2_PIN, HAND11BTYPE>(strips[0]->leds, NUM_LEDS_1A, NUM_LEDS_1B).setCorrection(TypicalLEDStrip);

  //Couple 2
  FastLED.addLeds<WS2811, DATA3_PIN, HAND12ATYPE>(strips[1]->leds, 0,  NUM_LEDS_2A).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2811, DATA4_PIN, HAND12BTYPE>(strips[1]->leds, NUM_LEDS_2A, NUM_LEDS_2B).setCorrection(TypicalLEDStrip);


  //Hand 2
  //Couple 3
  FastLED.addLeds<WS2811, DATA5_PIN, HAND21ATYPE>(strips[2]->leds, 0,  NUM_LEDS_3A).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2811, DATA6_PIN, HAND21BTYPE>(strips[2]->leds, NUM_LEDS_3A, NUM_LEDS_3B).setCorrection(TypicalLEDStrip);

  //Couple 4
  if (TREE == 1) {
    if (CONTROLLER == 3) {
      //Controller 3 has physically switched data pins.
      FastLED.addLeds<WS2811, DATA8_PIN, HAND22ATYPE>(strips[3]->leds, 0, NUM_LEDS_4A).setCorrection(TypicalLEDStrip);
      FastLED.addLeds<WS2811, DATA7_PIN, HAND22BTYPE>(strips[3]->leds, NUM_LEDS_4A, NUM_LEDS_4B).setCorrection(TypicalLEDStrip);
    }
  }
  else {
    FastLED.addLeds<WS2811, DATA7_PIN, HAND22ATYPE>(strips[3]->leds, 0, NUM_LEDS_4A).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<WS2811, DATA8_PIN, HAND22BTYPE>(strips[3]->leds, NUM_LEDS_4A, NUM_LEDS_4B).setCorrection(TypicalLEDStrip);
  }
  Serial.println("Fininshed setting up led strips");
}

void printStartupDebug() {
  Serial.print("ID:\t");
  Serial.println(ID);
}

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  delay(2500);

  Serial.begin(9600);
  randomSeed(analogRead(0));
  setupLedStrips();
  setupHands();
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
  GlobalState = Normal;
  //fillStripWithColorTemp();
  delay(500);
  printStartupDebug();
  FastLED.show();
  Serial.println("Startup complete.");
}

void TestPulses() {
  long Time = millis();

  if ((lastTestPulseTime + randomPulseTime ) < Time)
  {
    //int strip = random(0, 1);
    Serial.println("Making test pulse");
    for (int i = 0; i < NUM_HANDS; i++) {
      hands[i]->makePulses(1);
    }
    randomPulseTime = random(1500, 4500);
    lastTestPulseTime = Time;
  }
}

void testLength() {
  for (int i = 100; i >= 3; i--) {
    for (int j = 0; j < NUM_STRIPS_TOTAL; j++)
      strips[j]->leds[i] = CHSV(225, 225, 225);
  }
  FastLED.show();
  Serial.println("test complete");
}
void checkFakeFlash() {
  long Time = millis();
  if (Time > lastFlash + fakeFlashTime) {
    GlobalState = Synchronized;
    lastFlash = Time;
  }
}

void loop() {
  if (testMode)
  {
    //checkFakeFlash();
    TestPulses();
  }
  else {
    readInput();
  }

  //Update state according to FPS.
  long currentTime = millis();
  if (currentTime >= (lastUpdate + (1000 / FPS))) {
    executeState();
    lastUpdate = millis();
    Serial.print("State execution time in millis:\t");
    Serial.println((lastUpdate - currentTime));

  }
};
