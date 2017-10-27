/* 
	Editor: http://www.visualmicro.com
			visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
			the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
			all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
			note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Teensy 3.2 / 3.1, Platform=teensy3, Package=teensy
*/

#define __HARDWARE_MK20dx256__
#define __HARDWARE_MK20DX256__
#define _VMDEBUG 1
#define ARDUINO 10804
#define ARDUINO_MAIN
#define printf iprintf
#define __TEENSY3__
#define __MK20DX256__
#define TEENSYDUINO 139
#define ARDUINO 10804
#define F_CPU {build.fcpu}
#define {build.usbtype}
#define LAYOUT_{build.keylayout}
void executeState();
void mPulse(double intensity);
void readInput();
void checkForStripType();
void printStartupDebug();
//
//
void pulseFade();
void valveBeat();
void beat();
void setColourForTreeBeat();
void resetStrips();
void fakeRestPulse();
void doRestPulse();
void makeRestPulse(int strip);
void deleteRestPulse(int i);
void doPulse();
void collapsePulse();
void makePulse(int strip, double intensity);
void deletePulses();
void deletePulse(int i);

#include "wprogram.h"
#include "glow.ino"
#include "Animations.ino"
#include "PulseManager.ino"
