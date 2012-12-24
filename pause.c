#include "stats_screen.h"
#include "myLib.h"

void pause();
enum {SPLASH, INSTRUCTIONS, GAME_OVERWORLD, GAME_ARENA, PAUSE, WIN, LOSE};
extern int state;
OBJ_ATTR shadowOAM[128];

int points = 5;
int selector_position = 0;

extern int playerHealth;
extern int player_damage;

void pause() {
	REG_DISPCTL = MODE0 | BG3_ENABLE | SPRITE_ENABLE;
	REG_BG3CNT = CBB(0) | SBB(28) | BG_SIZE3 | COLOR256;
	loadPalette(stats_screenPal);
	DMANow(3, (unsigned int*)stats_screenTiles, &CHARBLOCKBASE[0], stats_screenTilesLen);
	DMANow(3, (unsigned int*)stats_screenMap, &SCREENBLOCKBASE[28], stats_screenMapLen);

	//sets up selector arrow
	shadowOAM[127].attr0 = 56 | ATTR0_8BPP | ATTR0_SQUARE;
	shadowOAM[127].attr1 = 25 | ATTR1_SIZE8;
	shadowOAM[127].attr2 = 772;

	//sets up points remaining indicator
	shadowOAM[126].attr0 = 32 | ATTR0_8BPP | ATTR0_SQUARE;
	shadowOAM[126].attr1 = 106 | ATTR1_SIZE8;
	shadowOAM[126].attr2 = 800;

	//sets up vitality points indicator
	shadowOAM[125].attr0 = 57 | ATTR0_8BPP | ATTR0_SQUARE;
	shadowOAM[125].attr1 = 90 | ATTR1_SIZE8;
	shadowOAM[125].attr2 = 802;
	
	//sets up strength points indicator
	shadowOAM[124].attr0 = 73 | ATTR0_8BPP | ATTR0_SQUARE;
	shadowOAM[124].attr1 = 90 | ATTR1_SIZE8;
	shadowOAM[124].attr2 = 802;

	//Disable unused sprites
	int i;
	for (i=0; i<128; ++i) {
		OAM[i].attr0 = ATTR0_HIDE;
	}

	//This loop keeps the background from being redrawn continously
	int button_pushed = 0;
	while (!button_pushed) {
		OAM[127] = shadowOAM[127];
		OAM[126] = shadowOAM[126];
		OAM[125] = shadowOAM[125];
		OAM[124] = shadowOAM[124];

		shadowOAM[126].attr2 = 800 + (2 * points);

		key_poll();
		if (key_hit(BUTTON_A) || (key_hit(BUTTON_B)))
			button_pushed = 1;

		if (key_hit(BUTTON_UP) && (selector_position > 0)) {
			--selector_position;
			shadowOAM[127].attr0 = 56 | ATTR0_8BPP | ATTR0_SQUARE;
		}
		if (key_hit(BUTTON_DOWN) && (selector_position < 1)) {
			++selector_position;
			shadowOAM[127].attr0 = 72 | ATTR0_8BPP | ATTR0_SQUARE;
		}
		if (key_hit(BUTTON_RIGHT) && (selector_position == 0) && (points > 0)) {
			++playerHealth;
			--points;
			shadowOAM[125].attr2 = 800 + (2 * playerHealth);
		}
		if (key_hit(BUTTON_LEFT) && (selector_position == 0) && (playerHealth > 1)) {
			--playerHealth;
			++points;
			shadowOAM[125].attr2 = 800 + (2 * playerHealth);
		}
		if (key_hit(BUTTON_RIGHT) && (selector_position == 1) && (points > 0)) {
			++player_damage;
			--points;
			shadowOAM[124].attr2 = 800 + (2 * player_damage);
		}
		if (key_hit(BUTTON_LEFT) && (selector_position == 1) && (player_damage > 1)) {
			--player_damage;
			++points;
			shadowOAM[124].attr2 = 800 + (2 * player_damage);
		}
	}

	if (key_hit(BUTTON_A)) {
		int i;
		for (i=0; i<128; ++i) {
			OAM[i].attr0 = ATTR0_HIDE;
		}
		state = GAME_OVERWORLD;
	}
}
