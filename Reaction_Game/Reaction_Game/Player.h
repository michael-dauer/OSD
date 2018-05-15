#pragma once
#include <string>

class Player
{
public:
	std::string get_name() const;
	float get_points() const;
	void add_win();
	void add_draw();
	void reset_points();

	Player(std::string name);
	Player();
	
private:
	std::string name_;
	float points_;
};

