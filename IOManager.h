#pragma once
#include <vector>

class IO_Manager
{
public:
	void reserve(int pin);
	void release(int pin);

private:
	static std::vector<int> reserved_pins_;
};