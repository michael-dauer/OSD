// Michael Dauer, Marlene Glawischnig
//
// 1.5.2018 19:00 Uhr
// FIXME: Pin Klasse aus vorheriger Übung teilweise implementiert
//

#include "Reaction_game.h"
#include "piproxy.h"
#include "pin_config.h"
#include "json.hpp"
#include <fstream>
#include <iostream>

// for convenience
using json = nlohmann::json;

int main(void)
{
	// read a JSON file9
	std::ifstream i("..\\Reaction_Game\\pins.json");
	json pin_config;
	i >> pin_config;

	for (json::iterator it = pin_config.begin(); it != pin_config.end(); ++it) {
		std::cout << *it << '\n';
	}

	std::cout << pin_config["p1_led"] << std::endl;


	wiringPiSetup();

	pinMode(PLAYER1_BUTTON, INPUT);
	pinMode(PLAYER2_BUTTON, INPUT);

	Reaction_game game;
	game.play();
	
	return 0;
}
