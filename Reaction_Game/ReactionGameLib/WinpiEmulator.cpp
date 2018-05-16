#include "WinpiEmulator.h"

#include <Windows.h>
#include <iostream>
#include <thread>
#include <array>

void Win_pi_emulator::add_pin(int pin, bool is_output)
{
	if (is_output) {
		outputs_[pin] = false;
	}
	else
	{
		const int pin_count = inputs_.size();
		const int new_index = pin_count + 1;
		if (new_index > max_inputs)
		{
			throw std::logic_error("WINPI: Only 8 inputs currently supported!");
		}

		inputs_.push_back(pin);
		std::cout << "WINPI: Mapping pin " << pin << " to your keyboard's F" << new_index << "-key!" << std::endl;
	}
}

void Win_pi_emulator::set_state(int pin, bool state)
{
	if (outputs_.find(pin) == outputs_.end())
	{
		throw std::logic_error("WINPI: Pin is not an output!");
	}

	outputs_[pin] = state;
}

bool Win_pi_emulator::get_state(int pin)
{
	for (unsigned int idx = 0; idx < inputs_.size(); idx++)
	{
		if (pin == inputs_[idx]) {
			return GetAsyncKeyState(VK_F1 + idx) != 0;
		}
	}
	if(outputs_.find(pin) != outputs_.end())
	{
		return outputs_[pin];
	}

	throw std::logic_error("WINPI: Pin is not configured!");
}

void Win_pi_emulator::subscribe_falling(int pin, void(* callback)())
{
	for (int input : inputs_)
	{
		if (pin == input) {
			falling_callbacks_[pin] = callback;
			return;
		}
	}

	throw std::logic_error("WINPI: Can't subscribe to pin that is not an input!");
}

void Win_pi_emulator::subscribe_rising(int pin, void(* callback)())
{
	for (int input : inputs_)
	{
		if (pin == input) {
			rising_callbacks_[pin] = callback;
			return;
		}
	}

	throw std::logic_error("WINPI: Can't subscribe to pin that is not an input!");
}

Win_pi_emulator::~Win_pi_emulator()
{
	poller_active_ = false;
	poller_.join();
}

void Win_pi_emulator::trigger_rising(int i)
{
	int pin = inputs_[i];
	if(rising_callbacks_.find(pin) != rising_callbacks_.end())
	{
		rising_callbacks_[pin]();
	}
}

void Win_pi_emulator::trigger_falling(int index)
{
	int pin = inputs_[index];
	if (falling_callbacks_.find(pin) != falling_callbacks_.end())
	{
		falling_callbacks_[pin]();
	}
}

void Win_pi_emulator::keyboard_poller()
{
	std::array<bool, max_inputs> last_state{};
	std::array<bool, max_inputs> current_state{};

	while (poller_active_)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		const int input_size = inputs_.size();

		for(int i = 0; i < input_size; i++)
		{
			current_state[i] = GetAsyncKeyState(VK_F1 + i) != 0;
			if(current_state[i] && !last_state[i])
			{
				trigger_rising(i);
			}
			else if(!current_state[i] && last_state[i])
			{
				trigger_falling(i);
			}
		}

		last_state = current_state;
	}
}

Win_pi_emulator::Win_pi_emulator() : poller_active_{true}
{
	poller_ = std::thread{ [this] {keyboard_poller(); } };
}

