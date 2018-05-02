#include "Pi_out_pin.h"
#include <wiringPi.h>

//the function set(bool value) gets a boolian value und sets the state of the pin (variable state_)
void Pi_out_pin::set(bool value)
{
	digitalWrite(pinNr_, value ? HIGH : LOW);
}

//the function toggle() inverts the value of the variable state_
void Pi_out_pin::toggle()
{
	int value = digitalRead(pinNr_);
	digitalWrite(pinNr_, value==LOW ? HIGH : LOW);
}

//the function state() returns the value of the variable state_
bool  Pi_out_pin::state() const
{
	return digitalRead(pinNr_) == HIGH;
}

//constructor 
Pi_out_pin::Pi_out_pin(int pinNr, bool value) : pinNr_(pinNr)
{
	pinMode(pinNr_, OUTPUT);
	digitalWrite(pinNr_, value ? HIGH : LOW);
}
