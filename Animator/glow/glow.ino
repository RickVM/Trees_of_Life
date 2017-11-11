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


#include "FastLED.h"
#include "ledstrips.h"
#include "Hand.h"
#include "UART.h"
#include "I2C.h"
#include "Enums.h"


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
          fadeToBlackBy(strips[i]->leds, NUM_LEDS, FALL_FADER);
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
  int y = COM->getHand();//Could be 1 or 2
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
  FastLED.addLeds<WS2811, DATA1_PIN, GRB>(strips[0]->leds, 0, NUM_LEDS_1A).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2811, DATA2_PIN, GRB>(strips[0]->leds, NUM_LEDS_1A, NUM_LEDS_1B).setCorrection(TypicalLEDStrip);

  //Couple 2
  FastLED.addLeds<WS2811, DATA3_PIN, GRB>(strips[1]->leds, 0,  NUM_LEDS_2A).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2811, DATA4_PIN, GRB>(strips[1]->leds, NUM_LEDS_2A, NUM_LEDS_2B).setCorrection(TypicalLEDStrip);

  //Couple 3
  FastLED.addLeds<WS2811, DATA5_PIN, GRB>(strips[2]->leds, 0,  NUM_LEDS_3A).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2811, DATA6_PIN, GRB>(strips[2]->leds, NUM_LEDS_3A, NUM_LEDS_3B).setCorrection(TypicalLEDStrip);

  //Couple 4
  FastLED.addLeds<WS2811, DATA7_PIN, GRB>(strips[3]->leds, 0, NUM_LEDS_4A).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2811, DATA8_PIN, GRB>(strips[3]->leds, NUM_LEDS_4A, NUM_LEDS_4B).setCorrection(TypicalLEDStrip);

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
  //int strip = random(0, 1);
  if ((lastTestPulseTime + 2500 ) < Time)
  {
    Serial.println("Making test pulse");
    for (int i = 0; i < NUM_HANDS; i++) {
      hands[i]->testMakePulses(i);
      //hands[i]->makePulse(0, 1);
      //hands[i]->doPulse();
    }
    lastTestPulseTime = Time;
  }
}

void loop() {
  //Test case 1
  //GlobalState = Synchronized;
  //Test case 2
  TestPulses();
    //readInput();

  long currentTime = millis();
  if (currentTime >= (lastUpdate + (1000 / FPS))) {

    executeState();
    lastUpdate = millis();
  }
};
