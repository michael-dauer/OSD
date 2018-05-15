#pragma once
#include <string>

class Player
{
public:
	std::string get_name(void) const;
	float get_points(void) const;
	void add_win(void);
	void add_draw(void);
	void reset_points(void);

	Player(std::string name);
	
private:
	std::string name_;
	float points_;
};

