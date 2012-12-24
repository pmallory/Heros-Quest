#include "walking_sprite.h"
#include "battleBG.h"
#include "battleFG.h"
#include "myLib.h"
#include "battleLoop.h"
#include "shootNoise.h"

//prototypes
void game_arena();
void arena_move(int);
void detect_collisions();
void draw_health_indicator();
void player_shoot();
void dragon_shoot();
void update_player_projectile();

extern int state;
enum {SPLASH, INSTRUCTIONS, GAME_OVERWORLD, GAME_ARENA, PAUSE, WIN, LOSE};

extern int easyMode;

OBJ_ATTR shadowOAM[128];
int playerCol = 0;
const int playerRow = 96; // Ground level
int projectileCol;
int fireCol;
enum {LEFT, RIGHT};
extern int characterCVel;
int projectile_available = 1; //if nonzero player can shoot.
int battle_in_progress = 1;
int player_damage = 1;

int cloudOffset = 0; //Cloud layer offset
int loopCounter = 0;

int playerHealth = 1;
int dragonHealth = 10;

void game_arena() {
	playSoundA(battleLoop, BATTLELOOPLEN, BATTLELOOPFREQ, 1);
	
	//Sets up display register
	REG_DISPCTL = MODE0 | BG3_ENABLE| BG2_ENABLE | SPRITE_ENABLE;
	
	//Sets up ground BG layer
	REG_BG2CNT = CBB(0) | SBB(28) | BG_SIZE0 | COLOR256;
	loadPalette(battleBGPal);
	DMANow(3, (unsigned int*)battleFGTiles, &CHARBLOCKBASE[0], battleFGTilesLen);
	DMANow(3, (unsigned int*)battleFGMap, &SCREENBLOCKBASE[28], battleFGMapLen);
	
	//Sets up sky BG layer
	REG_BG3CNT = CBB(2) | SBB(26) | BG_SIZE0 | COLOR256;
	//loadPalette(battleFGPal);
	DMANow(3, (unsigned int*)battleBGTiles, &CHARBLOCKBASE[2], battleBGTilesLen);
	DMANow(3, (unsigned int*)battleBGMap, &SCREENBLOCKBASE[26], battleBGMapLen);
	
	//Sets up sprite sheet and palette
	DMANow(3, (unsigned int*)walking_spriteTiles, &CHARBLOCKBASE[4], walking_spriteTilesLen/2);
	DMANow(3, (unsigned int*)walking_spritePal, SPRITE_PALETTE, walking_spritePalLen/2);
	int i;
	for (i=0; i<128; ++i) {
		OAM[i].attr0 = ATTR0_HIDE;
	}

	//Sets up character sprite
	shadowOAM[0].attr0 = playerRow | ATTR0_8BPP | ATTR0_SQUARE;
	shadowOAM[0].attr1 = playerCol | ATTR1_SIZE32;
	shadowOAM[0].attr2 = OFFSET(12,0,32);

	//sets up dragon sprite
	shadowOAM[1].attr0 =  80 | ATTR0_8BPP | ATTR0_SQUARE;
	shadowOAM[1].attr1 = 176 | ATTR1_SIZE64 | ATTR1_HFLIP;
	shadowOAM[1].attr2 = OFFSET(16,0,32);

	//sets up player health indicator sprites. OAM slots 50-59 are
	//available for these sprites.
	for (i=0; i<playerHealth; ++i) {
		shadowOAM[50+i].attr0 =  1 | ATTR0_8BPP | ATTR0_SQUARE;
		shadowOAM[50+i].attr1 =  (1+8*i) | ATTR1_SIZE8;
		shadowOAM[50+i].attr2 = 768;
	}

	//sets up dragon health indicator sprites. OAM slots 60-69 are
	//available for these sprites.
	for (i=0; i<dragonHealth; ++i) {
		shadowOAM[60+i].attr0 =  1 | ATTR0_8BPP | ATTR0_SQUARE;
		shadowOAM[60+i].attr1 =  (150+8*i) | ATTR1_SIZE8;
		shadowOAM[60+i].attr2 = 768;
	}

	//sets up player projectile sprite
	shadowOAM[13].attr0 = ATTR0_HIDE | ATTR0_8BPP | ATTR0_SQUARE;
	shadowOAM[13].attr1 = ATTR1_SIZE8;
	shadowOAM[13].attr2 = 770;

	//sets up dragon's breath sprite
	shadowOAM[14].attr0 = ATTR0_HIDE | ATTR0_8BPP | ATTR0_SQUARE;
	shadowOAM[14].attr1 = ATTR1_SIZE16;
	shadowOAM[14].attr2 = 528;

	while(battle_in_progress) {
		++loopCounter;
		key_poll();

		if (!key_held(BUTTON_UP) && !key_held(BUTTON_DOWN) && key_held(BUTTON_LEFT))
			arena_move(LEFT);		
		if (!key_held(BUTTON_UP) && !key_held(BUTTON_DOWN) && key_held(BUTTON_RIGHT))
			arena_move(RIGHT);
		if (key_hit(BUTTON_A))
			player_shoot();

		draw_health_indicator();
		dragon_shoot();		
		update_player_projectile();
		detect_collisions();

		//Scrolls cloud layer
		if (!(loopCounter % 42)) {
			++cloudOffset;
			REG_BG3HOFS = cloudOffset;
		}

		if ((0 >= playerHealth) || (0 >= dragonHealth))
			battle_in_progress = 0;

		//Copies shadowOAM into OAM
		shadowOAM[0].attr0 = (ROWMASK & playerRow) | ATTR0_8BPP | ATTR0_SQUARE;
		shadowOAM[0].attr1 = (COLMASK & playerCol) | ATTR1_SIZE32;

		waitForVblank();
		for (i=0; i<128; ++i) {
			OAM[i] = shadowOAM[i];
			OAM[127].attr0 = ATTR0_HIDE;
			OAM[126].attr0 = ATTR0_HIDE;
			OAM[125].attr0 = ATTR0_HIDE;
			OAM[124].attr0 = ATTR0_HIDE;
		}
	}

	if (0 >= dragonHealth)
		state = WIN;
	if (0 >= playerHealth)
		state = LOSE;
	dragonHealth = 10;

}

void arena_move(int direction) {
	static int step_counter = 0;
	
	switch(direction) {
		case LEFT:
			playerCol -= characterCVel;
			
			if (step_counter == 0)
				shadowOAM[0].attr2 = OFFSET(4,0,32);
			else if (step_counter == 5)
				shadowOAM[0].attr2 = OFFSET(4,8,32);
			else if (step_counter == 10)
				shadowOAM[0].attr2 = OFFSET(4,0,32);
			else if (step_counter == 15)
				shadowOAM[0].attr2 = OFFSET(4,16,32);

			break;
		case RIGHT:
			playerCol += characterCVel;
			
			if (step_counter == 0)
				shadowOAM[0].attr2 = OFFSET(12,0,32);
			else if (step_counter == 5)
				shadowOAM[0].attr2 = OFFSET(12,8,32);
			else if (step_counter == 10)
				shadowOAM[0].attr2 = OFFSET(12,0,32);
			else if (step_counter == 15)
				shadowOAM[0].attr2 = OFFSET(12,16,32);
				
			break;
	}

	if (step_counter < 20)
		++step_counter;
	else
		step_counter = 0;
}

void detect_collisions() {
	if (projectileCol > 176 && !projectile_available) {
		dragonHealth -= player_damage;
		shadowOAM[13].attr0 = ATTR0_HIDE;
		projectile_available = 1;
	}
}

void player_shoot() {
	if (projectile_available) {
		projectileCol = playerCol + 32;
		shadowOAM[13].attr0 = (playerRow + 16) | ATTR0_8BPP | ATTR0_SQUARE;
		shadowOAM[13].attr1 = projectileCol | ATTR1_SIZE8;
		shadowOAM[13].attr2 = 770;
		projectile_available = 0;

		playSoundB(shootNoise, SHOOTNOISELEN, SHOOTNOISEFREQ, 0);
	}
}

void dragon_shoot() {
	static int canShoot = 1;
	static int delay = 0;
	++delay;
	if (canShoot && !(delay%100)) {
		fireCol = 160;
		shadowOAM[14].attr0 = 104 | ATTR0_8BPP | ATTR0_SQUARE;
		shadowOAM[14].attr1 = fireCol | ATTR1_SIZE16;
		shadowOAM[14].attr2 = 528;
		canShoot = 0;
	}
	else if (canShoot){}//prevents the collision checks from being made
	else if (fireCol > playerCol + 32)
		shadowOAM[14].attr1 = (fireCol-=5) | ATTR1_SIZE16;
	else if (fireCol <= playerCol + 32) {
		if (!easyMode)
			--playerHealth;
		shadowOAM[14].attr0 = ATTR0_HIDE;
		canShoot = 1;
	}
	
}

void draw_health_indicator() {
	int i;

	for (i=50; i<70; ++i) {
		OAM[i].attr0 = ATTR0_HIDE;
	}

	//draws player health
	for(i=50; i<(playerHealth+50); ++i) {
		OAM[i].attr0 = 1 | ATTR0_8BPP | ATTR0_SQUARE;	
	}
	//draws dragon health
	for(i=60; i<(dragonHealth+60); ++i) {
		OAM[i].attr0 = 1 | ATTR0_8BPP | ATTR0_SQUARE;
	}
}

void update_player_projectile() {
	if (projectileCol < 240 && !projectile_available) {
		projectileCol += 5;
		shadowOAM[13].attr1 = (COLMASK & projectileCol) | ATTR1_SIZE8;
	} else {
		shadowOAM[13].attr0 = ATTR0_HIDE;
		projectile_available = 1;
	}
}
