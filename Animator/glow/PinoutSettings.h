#pragma once

#define TREE 1
#define CONTROLLER 2
const bool testMode = false;

#if TREE == 1 // Right tree

//wS2811 is GRB
//wS2811s is BRG
#if CONTROLLER == 1
#define HAND11ATYPE GRB
#define HAND11BTYPE BRG
#define HAND12ATYPE BRG
#define HAND12BTYPE BRG
#define HAND21ATYPE GRB
#define HAND21BTYPE BRG
#define HAND22ATYPE GRB
#define HAND22BTYPE BRG
#endif

#if CONTROLLER == 2
#define HAND11ATYPE GRB
#define HAND11BTYPE BRG
#define HAND12ATYPE GRB
#define HAND12BTYPE GRB
#define HAND21ATYPE GRB
#define HAND21BTYPE BRG
#define HAND22ATYPE GRB
#define HAND22BTYPE GRB
#endif

#if CONTROLLER == 3
//Dont forget to switch data 8 and 7
#define HAND11ATYPE GRB
#define HAND11BTYPE BRG
#define HAND12ATYPE GRB
#define HAND12BTYPE BRG
#define HAND21ATYPE BRG
#define HAND21BTYPE GRB
#define HAND22ATYPE BRG
#define HAND22BTYPE BRG

#endif

#else //Tree 2 left tree

#if CONTROLLER == 1
#define HAND11ATYPE GRB
#define HAND11BTYPE BRG
#define HAND12ATYPE GRB
#define HAND12BTYPE GRB
#define HAND21ATYPE BRG
#define HAND21BTYPE BRG
#define HAND22ATYPE BRG
#define HAND22BTYPE BRG
#endif

#if CONTROLLER == 2
#define HAND11ATYPE GRB
#define HAND11BTYPE GRB
#define HAND12ATYPE GRB
#define HAND12BTYPE GRB
#define HAND21ATYPE BRG
#define HAND21BTYPE GRB
#define HAND22ATYPE GRB
#define HAND22BTYPE GRB
#endif

#if CONTROLLER == 3
#define HAND11ATYPE GRB
#define HAND11BTYPE BRG
#define HAND12ATYPE GRB
#define HAND12BTYPE GRB
#define HAND21ATYPE GRB
#define HAND21BTYPE BRG
#define HAND22ATYPE GRB
#define HAND22BTYPE BRG

#endif
#endif

//Pinout
//Right RJ45 port when facing ports
#define DATA1_PIN 2 //Orange
#define DATA2_PIN 14 //Blue
#define DATA3_PIN 7 //Green
#define DATA4_PIN 8 //Brown

//Left RJ45 port when facing ports.
#define DATA5_PIN 6 //orange
#define DATA6_PIN 20 //Blue
#define DATA7_PIN 21 //Green
#define DATA8_PIN 5 //Brown

//Controller-Strip types
#define NUM_HANDS 2
#define NUM_STRIPS_PER_HAND 2 //When adjusting this, remember to also comment/uncomment FastLED.addleds in setup, and the ledstrip* array!!
#define NUM_STRIPS_TOTAL NUM_HANDS*NUM_STRIPS_PER_HAND

#define NUM_LEDS_1A 100
#define NUM_LEDS_1B 50
#define NUM_LEDS_2A 100
#define NUM_LEDS_2B 50
#define NUM_LEDS_3A 100
#define NUM_LEDS_3B 50
#define NUM_LEDS_4A 100
#define NUM_LEDS_4B 50

struct ledstrip {
  int nrOfLeds;
  //CRGB leds[100];
  CRGB* leds;
};
