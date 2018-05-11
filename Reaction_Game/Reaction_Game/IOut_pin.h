#pragma once
//Interface IOut_pin for the output pin

class IOut_pin
{
public:
	virtual void set(bool value) = 0;	//sets pin to a specific value

	virtual void toggle() = 0;			//inverts current value of the pin

	virtual bool state() const = 0;		//gets current value of the pin

	virtual ~IOut_pin() = 0;				//virtual deconstructor
};

