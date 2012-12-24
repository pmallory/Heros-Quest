#include "splashImg.h"
#include "instructionsImg.h"
#include "walking_sprite.h"
#include "myLib.h"

void instructions();
void splash();
void initialize_vars();

extern int state;
extern int easyMode;
enum {SPLASH, INSTRUCTIONS, GAME_OVERWORLD, GAME_ARENA, PAUSE, WIN, LOSE};

// These are variables from the other game states. Used to reset their values
// when starting a new game.
extern int playerCol;
extern int projectile_available;
extern int battle_in_progress;
extern int player_damage;
extern int cloudOffset;
extern int loopCounter;
extern int playerHealth;
extern int dragonHealth;
extern int characterRow;
extern int characterCol;
extern int characterWidth;
extern int characterHeight;
extern int characterRVel;
extern int characterCVel;
extern int facingLeft;
extern int in_battle;
extern int points;
extern int selector_position;
extern int easyMode;


void splash() {
	initialize_vars();

	REG_DISPCTL = MODE0 | BG3_ENABLE;
	REG_BG3CNT = CBB(0) | SBB(28) | BG_SIZE3 | COLOR256;
	loadPalette(splashPal);
	DMANow(3, (unsigned int*)splashTiles, &CHARBLOCKBASE[0], splashTilesLen);
	DMANow(3, (unsigned int*)splashMap, &SCREENBLOCKBASE[28], splashMapLen);

	//This loop keeps the background from being redrawn continously
	int button_pushed = 0;
	while (!button_pushed) {
		key_poll();
		if (key_hit(BUTTON_A) || (key_hit(BUTTON_B)))
			button_pushed = 1;
	}

	if (key_hit(BUTTON_A))
		state = INSTRUCTIONS;
	if (key_hit(BUTTON_B))
		state = PAUSE;

}

void instructions() {
	OBJ_ATTR shadowOAM[128];

	//Enables sprites
	REG_DISPCTL = MODE0 | BG3_ENABLE | SPRITE_ENABLE;
	REG_BG3CNT = CBB(0) | SBB(28) | BG_SIZE3 | COLOR256;

	//Loads sprites into memory
	DMANow(3, (unsigned int*)walking_spriteTiles, &CHARBLOCKBASE[4], walking_spriteTilesLen/2);
	//Loads sprite palette into memory
	DMANow(3, (unsigned int*)walking_spritePal, SPRITE_PALETTE, walking_spritePalLen/2);

	//Loads background stuff
	loadPalette(instructionsImgPal);
	DMANow(3, (unsigned int*)instructionsImgTiles, &CHARBLOCKBASE[0], instructionsImgTilesLen);
	DMANow(3, (unsigned int*)instructionsImgMap, &SCREENBLOCKBASE[28], instructionsImgMapLen);

	//Hides all sprites
	int i;
	for (i=0; i<128; ++i) {
		OAM[i].attr0 = ATTR0_HIDE;
	}

	shadowOAM[0].attr0 = 120 | ATTR0_8BPP | ATTR0_SQUARE;
	shadowOAM[0].attr1 = 177 | ATTR1_SIZE16;
	if (easyMode)
		shadowOAM[0].attr2 = 656;
	if (!easyMode)
		shadowOAM[0].attr2 = 592;
 
	//This loop keeps the background from being redrawn continously
	int button_pushed = 0;
	while (!button_pushed) {
		OAM[0] = shadowOAM[0];
		key_poll();
		if (key_hit(BUTTON_START))
			button_pushed = 1;

		if (key_hit(BUTTON_SELECT) && easyMode) {
			shadowOAM[0].attr2 = 592;
			easyMode = 0;
		}
		else if (key_hit(BUTTON_SELECT) && !easyMode) {
			shadowOAM[0].attr2 = 656;
			easyMode = 1;
		}
	}

	if (key_hit(BUTTON_START))
		state = SPLASH;
		
	loadPalette(splashPal);
}

void initialize_vars() {
	//vars from game_arena.c
	playerCol = 0;
	projectile_available = 1;
	battle_in_progress = 1;
	player_damage = 1;
	cloudOffset = 0;
	loopCounter = 0;
	playerHealth = 1;
	dragonHealth = 10;
	
	//vars from game_overworld.c
	characterRow = 60;
	characterCol = 10;
	characterWidth = 32;
	characterHeight = 32;
	characterRVel = 1;
	characterCVel = 1;
	facingLeft = 0;
	in_battle = 0;

	//vars from pause.c
	points = 5;
	selector_position = 0;

	//Disables all sprites
	int i;
	for (i=0; i<128; ++i) {
		OAM[i].attr0 = ATTR0_HIDE;
	}
}
