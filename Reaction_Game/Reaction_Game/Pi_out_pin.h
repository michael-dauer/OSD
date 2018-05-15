#pragma once
#include "IOut_pin.h"

class Pi_out_pin : public IOut_pin
{
public:
	void set(bool value) override;									//sets pin to a specific value
	void toggle() override;											//inverts current value of the pin
	bool state() const override;									//gets current value of the pin
	Pi_out_pin(int pinNr, bool value);		//constructor with parameters: pinnumber (pinNr) and output value (value)

private:
	int pin_nr_;
};
