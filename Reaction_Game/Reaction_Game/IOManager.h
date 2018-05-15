#pragma once
#include <vector>

class IO_Manager
{
public:
	void reserve(int pin);
	void release(int pin);
	bool is_reserved(int pin) const;

private:
	static std::vector<int> reserved_pins_;
};