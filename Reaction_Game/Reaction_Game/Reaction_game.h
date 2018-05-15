#pragma once
#include"Player.h"
#include <chrono>
#include "Pi_out_pin.h"

class Reaction_game
{
public:
	enum class Result { player1, player2, draw, timeout };

	void play();

private:
	Result play_round();

	void read_players();
	void show_results() const;

	static int read_num_rounds();
	static void reserve_pins();

	static void disable_ISR();
	static void player1_button_ISR();
	static void player2_button_ISR();
	static void blink(std::chrono::seconds, Result);
	static void all_leds_off();
	static std::chrono::duration<double> Reaction_game::random_wait_time();
	static void prepare_round();
	static void on_early_reaction(const Player& player, Pi_out_pin player_led);

	void handle_round_result(Result result);
	static void on_timeout(Pi_out_pin player1_led, Pi_out_pin player2_led);
	static void on_draw(Pi_out_pin player1_led, Pi_out_pin player2_led);
	void on_player1_wins(Pi_out_pin player1_led, Pi_out_pin player2_led) const;
	void on_player2_wins(Pi_out_pin player1_led, Pi_out_pin player2_led) const;

	static int64_t start_time_ms_;
	static int player1_reaction_time_;
	static int player2_reaction_time_;
	static bool player1_button_pressed_;
	static bool player2_button_pressed_;

	Player player1_;
	Player player2_;
};

