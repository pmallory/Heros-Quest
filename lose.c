#include "defeat.h"
#include "myLib.h"

void lose();

extern int state;
enum {SPLASH, INSTRUCTIONS, GAME_OVERWORLD, GAME_ARENA, PAUSE, WIN, LOSE};

void lose() {
	//Turns off sound
	REG_SOUNDCNT_X = !SND_ENABLED;

	waitForVblank();
	REG_BG3HOFS = 0;
	REG_BG3VOFS = 0;

	REG_DISPCTL = MODE0 | BG3_ENABLE;
	REG_BG3CNT = CBB(0) | SBB(28) | BG_SIZE3 | COLOR256;
	loadPalette(defeatPal);
	DMANow(3, (unsigned int*)defeatTiles, &CHARBLOCKBASE[0], defeatTilesLen);
	DMANow(3, (unsigned int*)defeatMap, &SCREENBLOCKBASE[28], defeatMapLen);

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

