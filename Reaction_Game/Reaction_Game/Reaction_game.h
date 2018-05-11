#pragma once
#include"Player.h"
#include <string>
#include <chrono>

class Reaction_game
{
public:
	enum class Result { player1, player2, draw, timeout };

	void play();

private:
	Result play_round(Player player1, Player player2);
	static void disable_ISR();
	static void player1_button_ISR();
	static void player2_button_ISR();
	void blink(std::chrono::seconds, Result);
	void all_leds_off();

	static int64_t start_time_ms_;
	static int player1_reaction_time_;
	static int player2_reaction_time_;
	static bool player1_button_pressed_;
	static bool player2_button_pressed_;
};

