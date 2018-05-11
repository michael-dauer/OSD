#include "Reaction_game.h"
#include "Player.h"
#include <wiringPi.h>
#include "pin_config.h"
#include "Pi_out_pin.h"
#include "IOManager.h"
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <random>
#include <thread>
#include <limits>
#include <cassert>

using namespace std;
using namespace std::chrono_literals;

constexpr auto TIME_START = 2s;
constexpr auto TIME_END = 3s;
constexpr auto TIME_BLINK = 5s;
constexpr auto MIN_WAIT_TIME = 5.0;
constexpr auto MAX_WAIT_TIME = 10.0;

void Reaction_game::play()
{
	string name = "John Doe";
	cout << "Player 1, please enter your name!" << endl;
	cin >> name;
	Player player1(name);

	name = "Jane Doe";
	cout << "Player 2, please enter your name!" << endl;
	cin >> name;
	Player player2(name);

	int number_of_rounds = 1;
	cout << "Enter the maximum number of rounds!" << endl;
	cin >> number_of_rounds;

	cout << "Game starts now!" << endl;

	IO_Manager io_manager;
	assert(!io_manager.is_reserved(PLAYER1_LED));
	io_manager.reserve(PLAYER1_LED);
	assert(!io_manager.is_reserved(PLAYER2_LED));
	io_manager.reserve(PLAYER2_LED);
	assert(!io_manager.is_reserved(REACTION_LED));
	io_manager.reserve(REACTION_LED);
	assert(!io_manager.is_reserved(START_ROUND_LED));
	io_manager.reserve(START_ROUND_LED);
	assert(!io_manager.is_reserved(PLAYER1_BUTTON));
	io_manager.reserve(PLAYER1_BUTTON);
	assert(!io_manager.is_reserved(PLAYER2_BUTTON));
	io_manager.reserve(PLAYER2_BUTTON);

	for (int current_round = 1; current_round <= number_of_rounds; ++current_round)
	{
		cout << "Round " << current_round << " starts as soon as you see the BLUE light!" << endl;
	
		Result result = play_round(player1, player2);
		switch (result)
		{
		case Result::player1:
			player1.add_win();
			break;
		case Result::player2:
			player2.add_win();
			break;
		case Result::draw:
			player2.add_draw();
			player1.add_draw();
			break;
		case Result::timeout:
			break;
		}
	}
	cout << player1.get_name() << " has " << player1.get_points() << " points." << endl;
	cout << player2.get_name() << " has " << player2.get_points() << " points." << endl;

	if (player1.get_points() > player2.get_points())
	{
		cout << player1.get_name() << " takes it all!! " << endl;
		blink(TIME_BLINK, Result::player1);
	}
	else if (player1.get_points() < player2.get_points())
	{
		cout << player2.get_name() << " takes it all!! " << endl;
		blink(TIME_BLINK, Result::player2);
	}
	else
	{
		cout << " DRAW!!" << endl;
		blink(TIME_BLINK, Result::draw);
	}
}


int64_t Reaction_game::start_time_ms_ = 0;
int Reaction_game::player1_reaction_time_ = 0;
int Reaction_game::player2_reaction_time_ = 0;
bool Reaction_game::player1_button_pressed_ = false;
bool Reaction_game::player2_button_pressed_ = false;
constexpr auto NAP = 10ms;

Reaction_game::Result Reaction_game::play_round(Player player1, Player player2)
{
	const unsigned int seed = time(NULL);
	mt19937_64 random_number_generator(seed);
	uniform_real_distribution<double> uniform(MIN_WAIT_TIME, MAX_WAIT_TIME);

	Pi_out_pin player1_led(PLAYER1_LED, false);
	Pi_out_pin player2_led(PLAYER2_LED, false);
	Pi_out_pin reaction_led(REACTION_LED, false);
	Pi_out_pin start_round_led(START_ROUND_LED, false);

	all_leds_off();

	this_thread::sleep_for(TIME_START);

	start_round_led.set(true);

	player1_button_pressed_ = false;
	player2_button_pressed_ = false;
	player1_reaction_time_ = numeric_limits<int>::max();
	player2_reaction_time_ = numeric_limits<int>::max();
	wiringPiISR(PLAYER1_BUTTON, INT_EDGE_FALLING, &player1_button_ISR);
	wiringPiISR(PLAYER2_BUTTON, INT_EDGE_FALLING, &player2_button_ISR);

	auto end = chrono::system_clock::now() + chrono::duration<double>(uniform(random_number_generator));
	while (!player1_button_pressed_ && !player2_button_pressed_ && !(chrono::system_clock::now()>end))
		this_thread::sleep_for(NAP);

	if (player1_button_pressed_)
	{
		cout << player1.get_name() << " you are way too nervous, DORK! " << endl;

		player1_led.set(true);
		this_thread::sleep_for(TIME_END);
		all_leds_off();
		return Result::player2;
	}
	else if (player2_button_pressed_)
	{
		cout << player2.get_name() << " you are way too nervous, DORK! " << endl;
		player2_led.set(true);
		this_thread::sleep_for(TIME_END);
		all_leds_off();
	}

	auto now = chrono::system_clock::now().time_since_epoch();
	start_time_ms_ = chrono::duration_cast<chrono::milliseconds>(now).count();

	reaction_led.set(true);
	end = chrono::system_clock::now() + TIME_END;
	while (!player1_button_pressed_ && !player2_button_pressed_ && !(chrono::system_clock::now()>end))
		this_thread::sleep_for(NAP);

	wiringPiISR(PLAYER1_BUTTON, INT_EDGE_FALLING, &disable_ISR);
	wiringPiISR(PLAYER2_BUTTON, INT_EDGE_FALLING, &disable_ISR);

	if (player1_button_pressed_ || player2_button_pressed_)
	{
		if (player1_reaction_time_ > player2_reaction_time_)
		{
			cout << player1.get_name() << " you LOOSER! " << endl;
			cout << "Come on, " << player2_reaction_time_ << " ms are to beat! " << endl;
			player1_led.set(false);
			player2_led.set(true);
			this_thread::sleep_for(TIME_END);
			all_leds_off();
			return Result::player2;
		}
		else if (player2_reaction_time_ > player1_reaction_time_)
		{
			cout << player2.get_name() << " you LOOSER! " << endl;
			cout << "Come on, " << player1_reaction_time_ << " ms are to beat! " << endl;
			player1_led.set(true);
			player2_led.set(false);
			this_thread::sleep_for(TIME_END);
			all_leds_off();
			return Result::player1;
		}
		else
		{
			cout << "DRAW!! " << endl;
			player1_led.set(true);
			player2_led.set(true);
			this_thread::sleep_for(TIME_END);
			all_leds_off();
			return Result::draw;
		}
	}
	else
	{
		cout << "Time out you SUPERBORES! Wake Up!! " << endl;
		player1_led.set(false);
		player2_led.set(false);
		this_thread::sleep_for(TIME_END);
		all_leds_off();
		return Result::timeout;
	}		
}

void Reaction_game::disable_ISR()
{
}

void Reaction_game::player1_button_ISR()
{
	auto now = chrono::system_clock::now().time_since_epoch();
	player1_reaction_time_ = chrono::duration_cast<chrono::milliseconds>(now).count() - start_time_ms_;
	player1_button_pressed_ = true;
}

void Reaction_game::player2_button_ISR()
{
	auto now = chrono::system_clock::now().time_since_epoch();
	player2_reaction_time_ = chrono::duration_cast<chrono::milliseconds>(now).count() - start_time_ms_;
	player2_button_pressed_ = true;
}

constexpr auto ON_TIME = 0.5s;
constexpr auto OFF_TIME = 0.5s;

void Reaction_game::blink(chrono::seconds blink_time, Result result)
{
	Pi_out_pin player1_led(PLAYER1_LED, false);
	Pi_out_pin player2_led(PLAYER2_LED, false);

	auto end = chrono::system_clock::now() + blink_time;
	while (chrono::system_clock::now() <= end)
	{
		if (result == Result::player1 || result == Result::draw)
			player1_led.set(false);
		if (result == Result::player2 || result == Result::draw)
			player2_led.set(false);
		this_thread::sleep_for(OFF_TIME);
		if (result == Result::player1 || result == Result::draw)
			player1_led.set(true);
		if (result == Result::player2 || result == Result::draw)
			player2_led.set(true);
		this_thread::sleep_for(ON_TIME);
	}

	player1_led.set(false);
	player2_led.set(false);
}

void Reaction_game::all_leds_off()
{
	Pi_out_pin player1_led(PLAYER1_LED, false);
	Pi_out_pin player2_led(PLAYER2_LED, false);
	Pi_out_pin reaction_led(REACTION_LED, false);
	Pi_out_pin start_round_led(START_ROUND_LED, false);
}
