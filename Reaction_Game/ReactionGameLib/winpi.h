#pragma once

/*
* wiringPi.h:
*	Arduino like Wiring library for the Raspberry Pi.
*	Copyright (c) 2012-2016 Gordon Henderson
***********************************************************************
* This file is part of wiringPi:
*	https://projects.drogon.net/raspberry-pi/wiringpi/
*
*    wiringPi is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Lesser General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    wiringPi is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Lesser General Public License for more details.
*
*    You should have received a copy of the GNU Lesser General Public License
*    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
***********************************************************************
*/

#ifndef	TRUE
#  define	TRUE	(1==1)
#  define	FALSE	(!TRUE)
#include <array>
#include "WinpiEmulator.h"
#endif

// Handy defines

// wiringPi modes

#define	WPI_MODE_PINS		 0
#define	WPI_MODE_GPIO		 1
#define	WPI_MODE_GPIO_SYS	 2
#define	WPI_MODE_PHYS		 3
#define	WPI_MODE_PIFACE		 4
#define	WPI_MODE_UNINITIALISED	-1

// Pin modes

#define	INPUT			 0
#define	OUTPUT			 1
#define	PWM_OUTPUT		 2
#define	GPIO_CLOCK		 3
#define	SOFT_PWM_OUTPUT		 4
#define	SOFT_TONE_OUTPUT	 5
#define	PWM_TONE_OUTPUT		 6

#define	LOW			 0
#define	HIGH			 1

// Pull up/down/none

#define	PUD_OFF			 0
#define	PUD_DOWN		 1
#define	PUD_UP			 2

// PWM

#define	PWM_MODE_MS		0
#define	PWM_MODE_BAL		1

// Interrupt levels

#define	INT_EDGE_SETUP		0
#define	INT_EDGE_FALLING	1
#define	INT_EDGE_RISING		2
#define	INT_EDGE_BOTH		3

// Pi model types and version numbers
//	Intended for the GPIO program Use at your own risk.

#define	PI_MODEL_A		0
#define	PI_MODEL_B		1
#define	PI_MODEL_AP		2
#define	PI_MODEL_BP		3
#define	PI_MODEL_2		4
#define	PI_ALPHA		5
#define	PI_MODEL_CM		6
#define	PI_MODEL_07		7
#define	PI_MODEL_3		8
#define	PI_MODEL_ZERO		9

#define	PI_VERSION_1		0
#define	PI_VERSION_1_1		1
#define	PI_VERSION_1_2		2
#define	PI_VERSION_2		3

#define	PI_MAKER_SONY		0
#define	PI_MAKER_EGOMAN		1
#define	PI_MAKER_MBEST		2
#define	PI_MAKER_UNKNOWN	3

extern const char *piModelNames[16];
extern const char *piRevisionNames[16];
extern const char *piMakerNames[16];
extern const int   piMemorySize[8];


//	Intended for the GPIO program Use at your own risk.

// Threads

#define	PI_THREAD(X)	void *X (void *dummy)

// Failure modes

#define	WPI_FATAL	(1==1)
#define	WPI_ALMOST	(1==2)


// wiringPiNodeStruct:
//	This describes additional device nodes in the extended wiringPi
//	2.0 scheme of things.
//	It's a simple linked list for now, but will hopefully migrate to 
//	a binary tree for efficiency reasons - but then again, the chances
//	of more than 1 or 2 devices being added are fairly slim, so who
//	knows....

struct wiringPiNodeStruct
{
	int     pinBase;
	int     pinMax;

	int          fd;	// Node specific
	unsigned int data0;	//  ditto
	unsigned int data1;	//  ditto
	unsigned int data2;	//  ditto
	unsigned int data3;	//  ditto

	void(*pinMode)         (struct wiringPiNodeStruct *node, int pin, int mode);
	void(*pullUpDnControl) (struct wiringPiNodeStruct *node, int pin, int mode);
	int(*digitalRead)     (struct wiringPiNodeStruct *node, int pin);
	void(*digitalWrite)    (struct wiringPiNodeStruct *node, int pin, int value);
	void(*pwmWrite)        (struct wiringPiNodeStruct *node, int pin, int value);
	int(*analogRead)      (struct wiringPiNodeStruct *node, int pin);
	void(*analogWrite)     (struct wiringPiNodeStruct *node, int pin, int value);

	struct wiringPiNodeStruct *next;
};

//struct wiringPiNodeStruct *wiringPiNodes;


// Function prototypes
//	c++ wrappers thanks to a comment by Nick Lott
//	(and others on the Raspberry Pi forums)


	// Data

	// Internal

int wiringPiFailure(int fatal, const char *message, ...);

// Core wiringPi functions

struct wiringPiNodeStruct *wiringPiFindNode(int pin);
struct wiringPiNodeStruct *wiringPiNewNode(int pinBase, int numPins);

int  wiringPiSetup(void);
int  wiringPiSetupSys(void);
int  wiringPiSetupGpio(void);
int  wiringPiSetupPhys(void);

void pinModeAlt(int pin, int mode);
void pinMode(int pin, int mode);
void pullUpDnControl(int pin, int pud);
int  digitalRead(int pin);
void digitalWrite(int pin, int value);
void pwmWrite(int pin, int value);
int  analogRead(int pin);
void analogWrite(int pin, int value);

// PiFace specifics 
//	(Deprecated)

int  wiringPiSetupPiFace(void);
int  wiringPiSetupPiFaceForGpioProg(void);	// Don't use this - for gpio program only

											// On-Board Raspberry Pi hardware specific stuff

int  piBoardRev(void);
void piBoardId(int *model, int *rev, int *mem, int *maker, int *overVolted);
int  wpiPinToGpio(int wpiPin);
int  physPinToGpio(int physPin);
void setPadDrive(int group, int value);
int  getAlt(int pin);
void pwmToneWrite(int pin, int freq);
void digitalWriteByte(int value);
unsigned int  digitalReadByte(void);
void pwmSetMode(int mode);
void pwmSetRange(unsigned int range);
void pwmSetClock(int divisor);
void gpioClockSet(int pin, int freq);

// Interrupts
//	(Also Pi hardware specific)

int  waitForInterrupt(int pin, int mS);
int  wiringPiISR(int pin, int mode, void(*function)(void));

// Threads

int  piThreadCreate(void *(*fn)(void *));
void piLock(int key);
void piUnlock(int key);

// Schedulling priority

int piHiPri(const int pri);

// Extras from arduino land

void         delay(unsigned int howLong);
void         delayMicroseconds(unsigned int howLong);
unsigned int millis(void);
unsigned int micros(void);

Win_pi_emulator& get_emulator();