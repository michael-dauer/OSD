// Michael Dauer, Marlene Glawischnig
//
// 1.5.2018 19:00 Uhr
// FIXME: Pin Klasse aus vorheriger Übung teilweise implementiert
//

#include "Reaction_game.h"
#include "piproxy.h"
#include "pin_config.h"

int main(void)
{	
	wiringPiSetup();

	pinMode(PLAYER1_BUTTON, INPUT);
	pinMode(PLAYER2_BUTTON, INPUT);

	Reaction_game game;
	game.play();
	
	return 0;
}
