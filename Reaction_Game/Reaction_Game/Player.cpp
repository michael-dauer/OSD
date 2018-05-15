#include "Player.h"

using namespace std;

string Player::get_name(void) const
{
	return name_;
}

float Player::get_points(void) const
{
	return points_;
}

void Player::add_win(void)
{
	points_ += 1.0f;
}

void Player::add_draw(void)
{
	points_ += 0.5f;
}

void Player::reset_points(void)
{
	points_ = 0.0f;
}

Player::Player(string name) : name_(name), points_(0.0f)
{
}
