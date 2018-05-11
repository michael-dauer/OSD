#include "IOManager.h"
#include <algorithm>

std::vector<int> IO_Manager::reserved_pins_;

void IO_Manager::reserve(int pin)
{
	if (((0 <= pin) && (pin <= 17)) || ((21 <= pin) && (pin <= 31)))
	{
		reserved_pins_.push_back(pin);
	}
}

void IO_Manager::release(int pin)
{
	std::vector<int>::iterator it;
	it = find(reserved_pins_.begin(), reserved_pins_.end(), pin);
	
	if (it != reserved_pins_.end()) 
	{
		reserved_pins_.erase(it);
	}
}

bool IO_Manager::is_reserved(int pin)
{
	std::vector<int>::iterator it;
	it = find(reserved_pins_.begin(), reserved_pins_.end(), pin);
	
	return it != reserved_pins_.end();
}
