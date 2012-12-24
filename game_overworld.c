#include "walking_sprite.h"
#include "overworld_map.h"
#include "pauseIMG.h"
#include "myLib.h"
#include "soundLoop.h"

//prototypes
void check_for_battle();
void game_overworld();
void overworld_update();
void overworld_draw();
void move();
void pause_graphic(int, int);
int updateNeeded();
void updateBGBig();

OBJ_ATTR shadowOAM[128];

enum {UP, DOWN, LEFT, RIGHT};

extern int state;
enum {SPLASH, INSTRUCTIONS, GAME_OVERWORLD, GAME_ARENA, PAUSE, WIN, LOSE};

int characterRow = 60;
int characterCol = 10;
int characterWidth = 32;
int characterHeight = 32;
int characterRVel = 1;
int characterCVel = 1;
int facingLeft = 0;

int sbbX, sbbY, hoff, voff;

int in_battle = 0;

void game_overworld() {
	//Turns on sound
	REG_SOUNDCNT_X = SND_ENABLED;
	playSoundA(soundLoop, SOUNDLOOPLEN*0.420, SOUNDLOOPFREQ, 1);

	//Enables sprites
	REG_DISPCTL = MODE0 | BG3_ENABLE | SPRITE_ENABLE;
	REG_BG3CNT = CBB(0) | SBB(28) | BG_SIZE3 | COLOR256;

	//Loads sprites into memory
	DMANow(3, (unsigned int*)walking_spriteTiles, &CHARBLOCKBASE[4], walking_spriteTilesLen/2);
	//Loads sprite palette into memory
	DMANow(3, (unsigned int*)walking_spritePal, SPRITE_PALETTE, walking_spritePalLen/2);

	//Loads background stuff
	loadPalette(overworld_mapPal);
	DMANow(3, (unsigned int*)overworld_mapTiles, &CHARBLOCKBASE[0], overworld_mapTilesLen);
	DMANow(3, (unsigned int*)overworld_mapMap, &SCREENBLOCKBASE[28], 8192);

	hoff = 0;
	voff = 0;
	sbbX = 0;
	sbbY = 0;

	
	//Hides all sprites
	int i;
	for (i=0; i<128; ++i) {
		OAM[i].attr0 = ATTR0_HIDE;
	}

	//Sets up character sprite
	shadowOAM[0].attr0 = characterRow | ATTR0_8BPP | ATTR0_SQUARE;
	shadowOAM[0].attr1 = characterCol | ATTR1_SIZE32;
	shadowOAM[0].attr2 = 840;

	//sets up dragon sprite
	shadowOAM[1].attr2 = OFFSET(16,0,32);

	while(!in_battle) {
		key_poll();

		// Checks directional buttons in a way that prevents the player from
		// moving diagonaly. A series of else-ifs would look prettier but then the
		// last item in the else-if chain would take 4 times as long to get to as the
		// first which results in the character moving at noticeably different speeds
		// when going in different directions. The boolean expressions are also
		// arranged to take advantage of short circuiting. Hella nifty.
		if (!key_held(BUTTON_LEFT) && !key_held(BUTTON_RIGHT) && key_held(BUTTON_UP))
			move(UP);		
		if (!key_held(BUTTON_UP) && !key_held(BUTTON_DOWN) && key_held(BUTTON_LEFT))
			move(LEFT);		
		if (!key_held(BUTTON_UP) && !key_held(BUTTON_DOWN) && key_held(BUTTON_RIGHT))
			move(RIGHT);
		if (!key_held(BUTTON_LEFT) && !key_held(BUTTON_RIGHT) && key_held(BUTTON_DOWN))
			move(DOWN);

		if(key_hit(BUTTON_START))
			pause_graphic(hoff, voff);

		// Update the shadowOAM with new player coordinates
		shadowOAM[0].attr0 = (ROWMASK & characterRow) | ATTR0_8BPP | ATTR0_SQUARE;
		shadowOAM[0].attr1 = (COLMASK & characterCol) | ATTR1_SIZE32;
		if (facingLeft)
			shadowOAM[0].attr1 |= ATTR1_HFLIP;
		
		// Positions the dragon so that it appears stationary
		if (hoff > 140 && voff < 64) {
			shadowOAM[1].attr0 = (ROWMASK & (-voff+275)) | ATTR0_8BPP | ATTR0_SQUARE;
			shadowOAM[1].attr1 = (COLMASK & (-hoff+378)) | ATTR1_SIZE64;
		}
		else
			OAM[1].attr0 = ATTR0_HIDE;

		check_for_battle();
		
		// Copy shadowOAM into actual OAM during vblank
		waitForVblank();
		REG_BG3HOFS = hoff;
		REG_BG3VOFS = voff;
		OAM[0] = shadowOAM[0];
		OAM[1] = shadowOAM[1];
	}

	if (in_battle)
		state = GAME_ARENA;
}

void check_for_battle() {
	if (hoff > 140 &&
		voff < 64 &&
		characterCol > (-hoff+346) &&
		characterCol < (-hoff+410) &&
		characterRow < (-voff+70)
		) in_battle = 1;
}

void move(int direction) {
	static int step_counter = 0;

	switch(direction) {
		case UP:
			if (characterRow < 56 && voff >= 0)
				voff -= characterRVel;
			else if (characterRow >= 0)
				characterRow -= characterRVel;			

			if (step_counter == 0)
				shadowOAM[0].attr2 = OFFSET(8,0,32);
			else if (step_counter == 5)
				shadowOAM[0].attr2 = OFFSET(8,8,32);
			else if (step_counter == 10)
				shadowOAM[0].attr2 = OFFSET(8,0,32);
			else if (step_counter == 15)
				shadowOAM[0].attr2 = OFFSET(8,16,32);

			facingLeft = 0;

			break;
		case DOWN:
			if (characterRow > 72 && voff <= 352) //352 = height of map - height of screen
				voff += characterRVel;
			else if (characterRow < 128) //128 = height of screen - height of player sprite
				characterRow += characterRVel;
			
			if (step_counter == 0)
				shadowOAM[0].attr2 = 840;
			else if (step_counter == 5)
				shadowOAM[0].attr2 = 832;
			else if (step_counter == 10)
				shadowOAM[0].attr2 = 840;
			else if (step_counter == 15)
				shadowOAM[0].attr2 = 848;

			facingLeft = 0;
			
			break;
		case LEFT:
			if (characterCol < 96 && hoff >= 0)
				hoff -= characterCVel;
			else if (characterCol >= 0)
				characterCol -= characterCVel;
							
			if (step_counter == 0) 
				shadowOAM[0].attr2 = OFFSET(12,0,32);
			else if (step_counter == 5)
				shadowOAM[0].attr2 = OFFSET(12,8,32);
			else if (step_counter == 10)
				shadowOAM[0].attr2 = OFFSET(12,0,32);
			else if (step_counter == 15)
				shadowOAM[0].attr2 = OFFSET(12,16,32);

			facingLeft = 1;

			break;
		case RIGHT:
			if (characterCol > 122 && hoff <= 272) //272 = width of map - width of screen
				hoff += characterCVel;
			else if (characterCol < 208)
				characterCol += characterCVel;
			
			if (step_counter == 0)
				shadowOAM[0].attr2 = OFFSET(12,0,32);
			else if (step_counter == 5)
				shadowOAM[0].attr2 = OFFSET(12,8,32);
			else if (step_counter == 10)
				shadowOAM[0].attr2 = OFFSET(12,0,32);
			else if (step_counter == 15)
				shadowOAM[0].attr2 = OFFSET(12,16,32);

			facingLeft = 0;
				
			break;
	}

	if (step_counter < 20)
		++step_counter;
	else
		step_counter = 0;

}

void pause_graphic(int oldHoff, int oldVoff) {
	//Displays pause graphic
	REG_DISPCTL = MODE0 | BG3_ENABLE;
	REG_BG3CNT = CBB(0) | SBB(28) | BG_SIZE3 | COLOR256;
	loadPalette(pauseIMGPal);
	DMANow(3, (unsigned int*)pauseIMGTiles, &CHARBLOCKBASE[0], pauseIMGTilesLen);
	DMANow(3, (unsigned int*)pauseIMGMap, &SCREENBLOCKBASE[28], pauseIMGMapLen);
	REG_BG3HOFS = 0;
	REG_BG3VOFS = 0;

	//waits until start is pushed
	int button_pushed = 0;
	while (!button_pushed) {
		key_poll();
		if (key_hit(BUTTON_START))
			button_pushed = 1;
	}

	//reloads map
	REG_DISPCTL = MODE0 | BG3_ENABLE | SPRITE_ENABLE;
	REG_BG3CNT = CBB(0) | SBB(28) | BG_SIZE3 | COLOR256;
	loadPalette(overworld_mapPal);
	DMANow(3, (unsigned int*)overworld_mapTiles, &CHARBLOCKBASE[0], overworld_mapTilesLen);
	DMANow(3, (unsigned int*)overworld_mapMap, &SCREENBLOCKBASE[28], 8192);

	//resets map offset
	REG_BG3HOFS = oldHoff;
	REG_BG3VOFS = oldVoff;
}
