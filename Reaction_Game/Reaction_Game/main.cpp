// Michael Dauer, Marlene Glawischnig
//
// 1.5.2018 19:00 Uhr
// FIXME: Pin Klasse aus vorheriger Übung teilweise implementiert
//

#include "Reaction_game.h"
#include <wiringPi.h>
#include <iostream>
#include "IOManager.h"
#include "pin_config.h"

int main(void)
{	
	IO_Manager io_manager;

	if (io_manager.is_reserved(13))
		std::cout << "Die 13 ist reserviert." << std::endl;
	else
		std::cout << "Die 13 ist frei." << std::endl;

	io_manager.reserve(13);

	if (io_manager.is_reserved(13))
		std::cout << "Die 13 ist reserviert." << std::endl;
	else
		std::cout << "Die 13 ist frei." << std::endl;

	io_manager.release(13);

	if (io_manager.is_reserved(13))
		std::cout << "Die 13 ist reserviert." << std::endl;
	else
		std::cout << "Die 13 ist frei." << std::endl;

	wiringPiSetup();

	pinMode(PLAYER1_BUTTON, INPUT);
	pinMode(PLAYER2_BUTTON, INPUT);

	Reaction_game game;
	game.play();
	
	return 0;
}
