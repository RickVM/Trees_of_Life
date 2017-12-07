#pragma once

#include "PinoutSettings.h"

//Colors vary based upon tree
#if TREE == 1
#define pulseHue 40
#define restPulseHue 90
#define flashR 255
#define flashG 70
#define flashB 0
#endif

#if TREE == 2
#define pulseHue 225
#define restPulseHue 90
#define flashR 255
#define flashG 0
#define flashB 30

#endif


#define BRIGHTNESS  125
#define FRAMES_PER_SECOND  40
#define FADER 40
#define PULSEFADER 40
#define FALL_FADER 30
#define FPS 30

//Beat Flash vars
#define nrOfBeats 4
#define nrOfFlashes 1
#define beatFrames 10 //Number of frames used in the brightness transition
#define beatDelay 18 //Delay between each frame
#define beatBrightness 100 // Total brightness the flash reaches.
#define flashSpeed 10
#define fakeFlashTime 310000

#define smallValveDelay 10
#define bigValveDelay 450


#define desyncTime 10000
//Rest pulse vars
//Pulse vars

#define RestPulseTime 2500

#define pulse5Intensity 1
#define pulse6Intensity 0.7
#define pulse7Intensity 0.5
#define pulse8Intensity 0.3
#define pulse9Intensity 0.25
#define pulse10Intensity 0.2
#define pulse11Intensity 0.1

