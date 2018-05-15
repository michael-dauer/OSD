#include "IOManager.h"
#include <algorithm>
#include <stdexcept>

std::vector<int> IO_Manager::reserved_pins_;

void IO_Manager::reserve(int pin)
{
	if (is_reserved(pin))
		throw std::logic_error("Trying to reserve pin that is already in use!");

	if (((0 <= pin) && (pin <= 17)) || ((21 <= pin) && (pin <= 31)))
	{
		reserved_pins_.push_back(pin);
	}
}

void IO_Manager::release(int pin)
{
	std::vector<int>::iterator it;
	it = find(reserved_pins_.begin(), reserved_pins_.end(), pin);

	if (!is_reserved(pin))
		throw std::logic_error("Trying to release pin that has not been reserved!");
	
	if (it != reserved_pins_.end()) 
	{
		reserved_pins_.erase(it);
	}
}

bool IO_Manager::is_reserved(int pin) const
{
	std::vector<int>::iterator it;
	it = find(reserved_pins_.begin(), reserved_pins_.end(), pin);
	
	return it != reserved_pins_.end();
}
