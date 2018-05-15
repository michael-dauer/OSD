#include "Player.h"

using namespace std;

string Player::get_name() const
{
	return name_;
}

float Player::get_points() const
{
	return points_;
}

void Player::add_win()
{
	points_ += 1.0f;
}

void Player::add_draw()
{
	points_ += 0.5f;
}

void Player::reset_points()
{
	points_ = 0.0f;
}

Player::Player(string name) : name_{ name }, points_{ 0.0f }
{
}

Player::Player() : points_{0.0f}
{
}
