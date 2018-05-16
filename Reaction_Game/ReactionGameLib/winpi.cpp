#include "winpi.h"

#include <thread>
#include <stdexcept>
#include <iostream>
#include "WinpiEmulator.h"

std::chrono::time_point<std::chrono::system_clock> start_time;

Win_pi_emulator winpi_emulator;

int piHiPri(const int pri)
{
	return 0;
}

void delay(unsigned int howLong)
{
	std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(howLong));
}

void delayMicroseconds(unsigned int howLong)
{
	std::this_thread::sleep_for(std::chrono::duration<int, std::micro>(howLong));
}

unsigned int millis(void)
{
	return 0;
}

unsigned int micros(void)
{
	return 0;
}

Win_pi_emulator& get_emulator()
{
	return winpi_emulator;
}

int wiringPiFailure(int fatal, const char * message, ...)
{
	throw std::runtime_error(message);
}

wiringPiNodeStruct * wiringPiFindNode(int pin)
{
	return nullptr;
}

wiringPiNodeStruct * wiringPiNewNode(int pinBase, int numPins)
{
	return nullptr;
}

int wiringPiSetup(void)
{
	start_time = std::chrono::system_clock::now();
	return 0;
}

int wiringPiSetupSys(void)
{
	start_time = std::chrono::system_clock::now();
	return 0;
}

int wiringPiSetupGpio(void)
{
	start_time = std::chrono::system_clock::now();
	return 0;
}

int wiringPiSetupPhys(void)
{
	start_time = std::chrono::system_clock::now(); 
	return 0;
}

void pinModeAlt(int pin, int mode)
{
}

void pinMode(int pin, int mode)
{
	std::cout << "WINPI: Setting pin mode for pin " << pin << " to mode " << mode << std::endl;
	winpi_emulator.add_pin(pin, mode == OUTPUT);
}

void pullUpDnControl(int pin, int pud)
{
}

int digitalRead(int pin)
{
	return winpi_emulator.get_state(pin);
}

void digitalWrite(int pin, int value)
{
	std::cout << "WINPI: Setting value " << value << " on pin " << pin << std::endl;
	winpi_emulator.set_state(pin, value != 0);
}

void pwmWrite(int pin, int value)
{
	std::cout << "WINPI: PWM write " << value << " on pin " << pin << std::endl;
}

int analogRead(int pin)
{
	return 0;
}

void analogWrite(int pin, int value)
{
	std::cout << "WINPI: Setting analog value " << value << " on pin " << value << std::endl;
}

int wiringPiSetupPiFace(void)
{
	return 0;
}

int wiringPiSetupPiFaceForGpioProg(void)
{
	return 0;
}

int piBoardRev(void)
{
	return 42;
}

void piBoardId(int * model, int * rev, int * mem, int * maker, int * overVolted)
{
}

int wpiPinToGpio(int wpiPin)
{
	return 0;
}

int physPinToGpio(int physPin)
{
	return 0;
}

void setPadDrive(int group, int value)
{
}

int getAlt(int pin)
{
	return 0;
}

void pwmToneWrite(int pin, int freq)
{
}

void digitalWriteByte(int value)
{
}

unsigned int digitalReadByte(void)
{
	return 0;
}

void pwmSetMode(int mode)
{
}

void pwmSetRange(unsigned int range)
{
}

void pwmSetClock(int divisor)
{
}

void gpioClockSet(int pin, int freq)
{
}

int waitForInterrupt(int pin, int mS)
{
	return 0;
}

int wiringPiISR(int pin, int mode, void(*function)(void))
{
	if(mode == INT_EDGE_BOTH || mode == INT_EDGE_FALLING)
	{
		winpi_emulator.subscribe_falling(pin, function);
	}
	if(mode == INT_EDGE_BOTH || mode == INT_EDGE_RISING)
	{
		winpi_emulator.subscribe_rising(pin, function);
	}
	return 0;
}

int piThreadCreate(void *(*fn)(void *))
{
	return 0;
}

void piLock(int key)
{
}

void piUnlock(int key)
{
}
