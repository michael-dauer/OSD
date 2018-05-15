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

constexpr auto time_start = 2s;
constexpr auto time_end = 3s;
constexpr auto time_blink = 5s;
constexpr auto min_wait_time = 5.0;
constexpr auto max_wait_time = 10.0;

int64_t Reaction_game::start_time_ms_ = 0;
int Reaction_game::player1_reaction_time_ = 0;
int Reaction_game::player2_reaction_time_ = 0;
bool Reaction_game::player1_button_pressed_ = false;
bool Reaction_game::player2_button_pressed_ = false;
constexpr auto NAP = 10ms;

void Reaction_game::play()
{
	read_players();

	const int number_of_rounds = read_num_rounds();

	cout << "Game starts now!" << endl;

	reserve_pins();

	for (int current_round = 1; current_round <= number_of_rounds; ++current_round)
	{
		cout << "Round " << current_round << " starts as soon as you see the BLUE light!" << endl;

		const Result result = play_round();
		handle_round_result(result);
	}

	show_results();
}

void Reaction_game::read_players()
{
	string name = "John Doe";
	cout << "Player 1, please enter your name!" << endl;
	cin >> name;
	player1_ = name;

	name = "Jane Doe";
	cout << "Player 2, please enter your name!" << endl;
	cin >> name;
	player2_ = name;
}

int Reaction_game::read_num_rounds()
{
	int number_of_rounds = 1;
	cout << "Enter the maximum number of rounds!" << endl;
	cin >> number_of_rounds;
	return number_of_rounds;
}

void Reaction_game::reserve_pins()
{
	IO_Manager io_manager;
	io_manager.reserve(PLAYER1_LED);
	io_manager.reserve(PLAYER2_LED);
	io_manager.reserve(REACTION_LED);
	io_manager.reserve(START_ROUND_LED);
	io_manager.reserve(PLAYER1_BUTTON);
	io_manager.reserve(PLAYER2_BUTTON);
}

void Reaction_game::show_results() const
{
	cout << player1_.get_name() << " has " << player1_.get_points() << " points." << endl;
	cout << player2_.get_name() << " has " << player2_.get_points() << " points." << endl;

	if (player1_.get_points() > player2_.get_points())
	{
		cout << player1_.get_name() << " takes it all!! " << endl;
		blink(time_blink, Result::player1);
	}
	else if (player1_.get_points() < player2_.get_points())
	{
		cout << player2_.get_name() << " takes it all!! " << endl;
		blink(time_blink, Result::player2);
	}
	else
	{
		cout << " DRAW!!" << endl;
		blink(time_blink, Result::draw);
	}
}

void Reaction_game::handle_round_result(Reaction_game::Result result)
{
	switch (result)
	{
	case Result::player1:
		player1_.add_win();
		break;
	case Result::player2:
		player2_.add_win();
		break;
	case Result::draw:
		player2_.add_draw();
		player1_.add_draw();
		break;
	case Result::timeout:
		break;
	}
}

void Reaction_game::on_timeout(Pi_out_pin player1_led, Pi_out_pin player2_led)
{
	cout << "Time out you SUPERBORES! Wake Up!! " << endl;
	player1_led.set(false);
	player2_led.set(false);
	this_thread::sleep_for(time_end);
	all_leds_off();
}

void Reaction_game::on_draw(Pi_out_pin player1_led, Pi_out_pin player2_led)
{
	cout << "DRAW!! " << endl;
	player1_led.set(true);
	player2_led.set(true);
	this_thread::sleep_for(time_end);
	all_leds_off();
}

void Reaction_game::on_player1_wins(Pi_out_pin player1_led, Pi_out_pin player2_led) const
{
	cout << player2_.get_name() << " you LOOSER! " << endl;
	cout << "Come on, " << player1_reaction_time_ << " ms are to beat! " << endl;
	player1_led.set(true);
	player2_led.set(false);
	this_thread::sleep_for(time_end);
	all_leds_off();
}

void Reaction_game::on_player2_wins(Pi_out_pin player1_led, Pi_out_pin player2_led) const
{
	cout << player1_.get_name() << " you LOOSER! " << endl;
	cout << "Come on, " << player2_reaction_time_ << " ms are to beat! " << endl;
	player1_led.set(false);
	player2_led.set(true);
	this_thread::sleep_for(time_end);
	all_leds_off();
}

chrono::duration<double> Reaction_game::random_wait_time()
{
	const unsigned int seed = time(nullptr);
	mt19937_64 random_number_generator(seed);
	uniform_real_distribution<double> uniform(min_wait_time, max_wait_time);
	return chrono::duration<double>(uniform(random_number_generator));
}

void Reaction_game::prepare_round()
{
	player1_button_pressed_ = false;
	player2_button_pressed_ = false;
	player1_reaction_time_ = numeric_limits<int>::max();
	player2_reaction_time_ = numeric_limits<int>::max();
	wiringPiISR(PLAYER1_BUTTON, INT_EDGE_FALLING, &player1_button_ISR);
	wiringPiISR(PLAYER2_BUTTON, INT_EDGE_FALLING, &player2_button_ISR);
}

void Reaction_game::on_early_reaction(const Player& player, Pi_out_pin player_led)
{
	cout << player.get_name() << " you are way too nervous, DORK! " << endl;

	player_led.set(true);
	this_thread::sleep_for(time_end);
	all_leds_off();
}

Reaction_game::Result Reaction_game::play_round()
{
	Pi_out_pin player1_led(PLAYER1_LED, false);
	Pi_out_pin player2_led(PLAYER2_LED, false);
	Pi_out_pin reaction_led(REACTION_LED, false);
	Pi_out_pin start_round_led(START_ROUND_LED, false);

	all_leds_off();

	this_thread::sleep_for(time_start);

	start_round_led.set(true);

	prepare_round();

	const auto wait_time = random_wait_time();
	const auto wait_end = chrono::system_clock::now() + wait_time;
	while (!player1_button_pressed_ && !player2_button_pressed_ && !(chrono::system_clock::now() > wait_end))
		this_thread::sleep_for(NAP);

	if (player1_button_pressed_)
	{
		on_early_reaction(player1_, player1_led);
		return Result::player2;
	}
	if (player2_button_pressed_)
	{
		on_early_reaction(player2_, player1_led);
		return Result::player1;
	}

	const auto now = chrono::system_clock::now().time_since_epoch();
	start_time_ms_ = chrono::duration_cast<chrono::milliseconds>(now).count();

	reaction_led.set(true);
	const auto reaction_end = chrono::system_clock::now() + time_end;
	while (!player1_button_pressed_ && !player2_button_pressed_ && !(chrono::system_clock::now() > reaction_end))
		this_thread::sleep_for(NAP);

	wiringPiISR(PLAYER1_BUTTON, INT_EDGE_FALLING, &disable_ISR);
	wiringPiISR(PLAYER2_BUTTON, INT_EDGE_FALLING, &disable_ISR);

	if (player1_button_pressed_ || player2_button_pressed_)
	{
		if (player1_reaction_time_ > player2_reaction_time_)
		{
			on_player2_wins(player1_led, player2_led);
			return Result::player2;
		}
		if (player2_reaction_time_ > player1_reaction_time_)
		{
			on_player1_wins(player1_led, player2_led);
			return Result::player1;
		}

		on_draw(player1_led, player2_led);
		return Result::draw;
	}

	on_timeout(player1_led, player2_led);
	return Result::timeout;
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

void Reaction_game::blink(chrono::seconds blink_time, Result result)
{
	constexpr auto ON_TIME = 0.5s;
	constexpr auto OFF_TIME = 0.5s;

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
