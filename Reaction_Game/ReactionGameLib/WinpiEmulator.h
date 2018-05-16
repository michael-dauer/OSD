#pragma once
#include <thread>
#include <vector>
#include <map>

class Win_pi_emulator {

public:
	Win_pi_emulator();

	void add_pin(int pin, bool is_output);
	void set_state(int pin, bool state);
	bool get_state(int pin);

	Win_pi_emulator(const Win_pi_emulator &other) = delete;
	Win_pi_emulator(Win_pi_emulator &&other) = default;

	void subscribe_falling(int pin, void(*callback)());
	void subscribe_rising(int pin, void(*callback)());

	Win_pi_emulator& operator=(const Win_pi_emulator& rhs) = delete;
	Win_pi_emulator& operator=(Win_pi_emulator&& rhs) = default;

	~Win_pi_emulator();
	void trigger_rising(int i);
	void trigger_falling(int i);

private:

	static constexpr int num_pins = 28;
	static constexpr int max_inputs = 8;

	std::vector<int> inputs_;
	std::map<int, bool> outputs_;
	std::map<int, void(*)()> rising_callbacks_;
	std::map<int, void(*)()> falling_callbacks_;
	bool poller_active_;
	std::thread poller_;

	void keyboard_poller();
};
