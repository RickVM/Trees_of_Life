#pragma once
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
#define NUM_LEDS_PER_STRIP 50
#define NUM_LEDS NUM_LEDS_PER_STRIP*NUM_STRIPS_PER_HAND

#define NUM_LEDS_1A 50
#define NUM_LEDS_1B 50
#define NUM_LEDS_2A 50
#define NUM_LEDS_2B 50
#define NUM_LEDS_3A 50
#define NUM_LEDS_3B 50
#define NUM_LEDS_4A 50
#define NUM_LEDS_4B 50


struct ledstrip {
  int nrOfLeds;
  CRGB leds[100];
  // CRGB* leds;
}; 
