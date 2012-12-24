#include "myLib.h"
#include "walking_sprite.h"

//prototypes
void splash();
void instructions();
void game_overworld();
void game_arena();
void pause();
void win();
void lose();

enum {SPLASH, INSTRUCTIONS, GAME_OVERWORLD, GAME_ARENA, PAUSE, WIN, LOSE};
int state = SPLASH; //Should be SPLASH

int easyMode = 0;

int main() {

	//Loads sprites into memory
	DMANow(3, (unsigned int*)walking_spriteTiles, &CHARBLOCKBASE[4], walking_spriteTilesLen/2);
	//Loads sprite palette into memory
	DMANow(3, (unsigned int*)walking_spritePal, SPRITE_PALETTE, walking_spritePalLen/2);

	setupInterrupts();
	setupSounds();

	while(1) {

		switch(state) {
		case SPLASH:
			splash();
			break;
		case INSTRUCTIONS:
			instructions();
			break;
		case GAME_OVERWORLD:
			game_overworld();
			break;
		case GAME_ARENA:
			game_arena();
			break;
		case PAUSE:
			pause();
			break;
		case WIN:
			win();
			break;
		case LOSE:
			lose();
			break;
		}

	} // while

	return 420;
}
