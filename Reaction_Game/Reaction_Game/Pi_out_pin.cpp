#include "Pi_out_pin.h"
#include <wiringPi.h>

//the function set(bool value) gets a boolian value und sets the state of the pin (variable state_)
void Pi_out_pin::set(bool value)
{
	digitalWrite(pin_nr_, value ? HIGH : LOW);
}

//the function toggle() inverts the value of the variable state_
void Pi_out_pin::toggle()
{
	int value = digitalRead(pin_nr_);
	digitalWrite(pin_nr_, value==LOW ? HIGH : LOW);
}

//the function state() returns the value of the variable state_
bool  Pi_out_pin::state() const
{
	return digitalRead(pin_nr_) == HIGH;
}

//constructor 
Pi_out_pin::Pi_out_pin(int pinNr, bool value) : pin_nr_(pinNr)
{
	pinMode(pin_nr_, OUTPUT);
	digitalWrite(pin_nr_, value ? HIGH : LOW);
}
