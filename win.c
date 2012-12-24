#include "victory.h"
#include "myLib.h"

void win();

extern int state;
enum {SPLASH, INSTRUCTIONS, GAME_OVERWORLD, GAME_ARENA, PAUSE, WIN, LOSE};

void win() {
	//Turns off sound
	REG_SOUNDCNT_X = !SND_ENABLED;

	waitForVblank();
	REG_BG3HOFS = 0;
	REG_BG3VOFS = 0;

	REG_DISPCTL = MODE0 | BG3_ENABLE;
	REG_BG3CNT = CBB(0) | SBB(28) | BG_SIZE3 | COLOR256;
	loadPalette(victoryPal);
	DMANow(3, (unsigned int*)victoryTiles, &CHARBLOCKBASE[0], victoryTilesLen);
	DMANow(3, (unsigned int*)victoryMap, &SCREENBLOCKBASE[28], victoryMapLen);

	//This loop keeps the background from being redrawn continously
	int button_pushed = 0;
	while (!button_pushed) {
		key_poll();
		if (key_hit(BUTTON_START))
			button_pushed = 1;
	}

	if (key_hit(BUTTON_START))
		state = SPLASH;
}
