#include "IOManager.h"
#include <algorithm>

void IO_Manager::reserve(int pin)
{
	reserved_pins_.push_back(pin);

}

void IO_Manager::release(int pin)
{
	std::vector<int>::iterator it;
	it = find(reserved_pins_.begin(), reserved_pins_.end(), pin);
	reserved_pins_.erase(it);

}
