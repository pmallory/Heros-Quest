#include "myLib.h"

unsigned short *videoBuffer = (u16 *)0x6000000;

unsigned short *frontBuffer = (u16 *)0x6000000;
unsigned short *backBuffer =  (u16 *)0x600A000;

DMA *dma = (DMA *)0x40000B0;

int scale_up(int in) {
	return in << 7;
}

int scale_down(int in) {
	return in >> 7;
}

void setPixel3(int row, int col, unsigned short color)
{
	videoBuffer[OFFSET(row, col, SCREENWIDTH)] = color;
}

void drawRect3(int row, int col, int height, int width, unsigned short color)
{
	unsigned short c = color;
	
	int i;
	for(i = 0; i < height; i++)
	{
		DMANow(3, &c, &videoBuffer[OFFSET(row+i, col, SCREENWIDTH)], (width) | DMA_SOURCE_FIXED);
	}
}

void drawImage3(const unsigned short* image, int row, int col, int height, int width)
{
	int i;
	for(i = 0; i < height; i++)
	{
		DMANow(3, (unsigned short*)&image[OFFSET(i,0, width)], &videoBuffer[OFFSET(row+i, col, SCREENWIDTH)], (width));
	}
}

void fillScreen3(unsigned short color)
{
	unsigned short c = color;

	DMANow(3, &c, videoBuffer, (240*160) | DMA_SOURCE_FIXED);
	
}

void setPixel4(int row, int col, unsigned char colorIndex)
{
	unsigned short pixels = videoBuffer[OFFSET(row, col/2, SCREENWIDTH/2)];

	if(col % 2 == 0) // even
	{
		pixels &= 0xFF << 8;
		videoBuffer[OFFSET(row, col/2, SCREENWIDTH/2)] = pixels | colorIndex;
	}
	else // odd
	{
		pixels &= 0xFF;
		videoBuffer[OFFSET(row, col/2, SCREENWIDTH/2)] = pixels | colorIndex << 8;
	}
}

void drawRect4(int row, int col, int height, int width, unsigned char colorIndex)
{
	unsigned short pixels = colorIndex << 8 | colorIndex;

	int r;
	for(r = 0; r < height; r++)
	{
		if(col % 2 == 0) // even starting col
		{
			DMANow(3, &pixels, &videoBuffer[OFFSET(row + r, col/2, SCREENWIDTH/2)], (width/2) | DMA_SOURCE_FIXED);	
			if(width % 2 == 1) // if width is odd
			{
				setPixel4(row+r, col+width - 1, colorIndex);
			}
		}
		else // old starting col
		{
			setPixel4(row+r, col, colorIndex);

			if(width % 2 == 1) // if width is odd
			{
				DMANow(3, &pixels, &videoBuffer[OFFSET(row + r, (col+1)/2, SCREENWIDTH/2)], (width/2) | DMA_SOURCE_FIXED);
			}
			else  // width is even
			{
				DMANow(3, &pixels, &videoBuffer[OFFSET(row + r, (col+1)/2, SCREENWIDTH/2)], ((width/2)-1) | DMA_SOURCE_FIXED);
				setPixel4(row+r, col+width - 1, colorIndex);
			}
		}
	}
	
}

void fillScreen4(unsigned char colorIndex)
{
	unsigned short pixels = colorIndex << 8 | colorIndex;
	DMANow(3, &pixels, videoBuffer, ((240 * 160)/2) | DMA_SOURCE_FIXED);
}

void drawBackgroundImage4(const unsigned short* image)
{
	DMANow(3, (unsigned short*)image, videoBuffer, ((240 * 160)/2));
}
void drawImage4(const unsigned short* image, int row, int col, int height, int width)
{
	if(col%2)
	{
		col++;
	}

	int r;
	for(r = 0; r < height; r++)
	{
		DMANow(3, (unsigned short*)&image[OFFSET(r,0,width/2)], &videoBuffer[OFFSET(row + r, col/2, SCREENWIDTH/2)], width/2);
	}
}

void drawSubImage4(const unsigned short* sourceImage, int sourceRow, int sourceCol, int sourceWidth, 
				   int row, int col, int height, int width)
{
	if(sourceCol%2)
	{
		sourceCol++;
	}

	if(col%2)
	{
		col++;
	}

	int r;
	for(r = 0; r < height; r++)
	{
		DMANow(3, (unsigned short*)&sourceImage[OFFSET(sourceRow + r, sourceCol/2,width/2)], &videoBuffer[OFFSET(row + r, col/2, SCREENWIDTH/2)], width/2);
	}
}

void loadPalette(const unsigned short* palette)
{
	DMANow(3, (unsigned short*)palette, PALETTE, 256);
}


void DMANow(int channel, void* source, void* destination, unsigned int control)
{
	dma[channel].src = source;
	dma[channel].dst = destination;
	dma[channel].cnt = DMA_ON | control;
}

void waitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}


void flipPage()
{
    if(REG_DISPCTL & BACKBUFFER)
    {
        REG_DISPCTL &= ~BACKBUFFER;
        videoBuffer = backBuffer;
    }
    else
    {
        REG_DISPCTL |= BACKBUFFER;
        videoBuffer = frontBuffer;
    }
}

extern u16 __key_curr, __key_prev;
u16 __key_curr=0, __key_prev=0;

inline void key_poll() {
	__key_prev= __key_curr;
	__key_curr= ~BUTTON_ADDRESS & KEY_MASK;
}

inline u32 key_curr_state() {return __key_curr;}
inline u32 key_prev_state()  {	return __key_prev;}
inline u32 key_is_down(u32 key) { return __key_curr & key;	}
inline u32 key_is_up(u32 key) {return ~__key_curr & key; }
inline u32 key_was_down(u32 key) { return __key_prev & key;	}
inline u32 key_was_up(u32 key)  {	return ~__key_prev & key; } 
inline u32 key_held(u32 key){ return ( __key_curr & __key_prev) & key; }
inline u32 key_hit(u32 key){ return ( __key_curr &~ __key_prev) & key; }
inline u32 key_released(u32 key) { return (~__key_curr & __key_prev) & key; }

int vblankCounter = 0;

int soundAStart = 0;  // Start when did the sound start
int soundALooping = 0; // should we loop the sound
char *soundA;        // the sound itself
int soundALength = 0;  // the length of the sound in samples
int soundADuration = 0;  // duration of the sound in vBlanks
int soundAFreq = 0;  // frequency of the sound
int soundAPlaying = 0;  // whether the sound is currently playing

int soundBStart = 0;
int soundBLooping = 0;
char *soundB;
int soundBLength = 0;
int soundBDuration = 0;
int soundBFreq = 0;
int soundBPlaying = 0;

void setupSounds()
{
	// COMPLETE THIS FUNCTION !!!
	// This function will enable sounds
	// uncomment and complete the following lines
	REG_SOUNDCNT_X = SND_ENABLED;

	REG_SOUNDCNT_H = SND_OUTPUT_RATIO_100 | //100% volume
					 DSA_OUTPUT_RATIO_100 | // 100% volume for channel A
					 DSA_OUTPUT_TO_BOTH |  // play sounds out of both speakers
					 DSA_TIMER0 |			// use timer 0
					 DSA_FIFO_RESET |		// enable FIFO reset
					 DSB_OUTPUT_RATIO_100 |
					 DSB_OUTPUT_TO_BOTH |
					 DSB_TIMER1 | 
					 DSB_FIFO_RESET;

	REG_SOUNDCNT_L = 0;
}

void playSoundA( const unsigned char* sound, int length, int frequency, int isLooping ) {

	if(soundAPlaying)
	{
		dma[1].cnt = 0; // turn off DMA
		REG_TM0CNT = 0; // turn off timer 0
		//return;
	}

	// COMPLETE THIS FUNCTION !!!
	// This function will play a sound out of sound channel A
	// fist compute the timer interval for the sound.  The number of cycles divided by the frequency of the sound
	int timerInterval = 16777216 / frequency;
	// then setup the DMA channel for this sound
	DMANow(1,
		   (unsigned int *)sound,
		   (unsigned int *)REG_FIFO_A,
		   DMA_32 | 
		   DMA_REPEAT |
		   DMA_DESTINATION_FIXED |
		   DMA_AT_REFRESH |
		   DMA_SOURCE_INCREMENT);

	// then setup the timer for this sound
	REG_TM0CNT = 0;   // turn timer 0 off
	REG_TM0D = -timerInterval;  // set the interval for the timer to count
	REG_TM0CNT = TIMER_ON;  // turn timer 0 back on

	// then start the timer

	// you may have to create additional global variables for these functions to work (to stop the sounds later)
	soundAStart = vblankCounter;
	soundALooping = isLooping;
	soundA = (char *)sound;
	soundALength = length;
					// time in seconds == length/frequency
					// time in vblanks == time in seconds * about 60
	soundADuration = ((length*60) / frequency) - ((length/frequency)*3) - 1;
	soundAFreq = frequency;

	soundAPlaying = 1; // 1 == true
}


void playSoundB( const unsigned char* sound, int length, int frequency, int isLooping ) {

	// COMPLETE THIS FUNCTION !!!

	// This function will play a sound out of sound channel B
	// refer to playSoundA instructions
	if(soundBPlaying)
	{
		dma[2].cnt = 0; // turn off DMA
		REG_TM1CNT = 0; // turn off timer 0
		//return;
	}

	// COMPLETE THIS FUNCTION !!!
	// This function will play a sound out of sound channel A
	// fist compute the timer interval for the sound.  The number of cycles divided by the frequency of the sound
	int timerInterval = 16777216 / frequency;
	// then setup the DMA channel for this sound
	DMANow(2,
		   (unsigned int *)sound,
		   (unsigned int *)REG_FIFO_B,
		   DMA_32 | 
		   DMA_REPEAT |
		   DMA_DESTINATION_FIXED |
		   DMA_AT_REFRESH |
		   DMA_SOURCE_INCREMENT);

	// then setup the timer for this sound
	REG_TM1CNT = 0;   // turn timer 0 off
	REG_TM1D = -timerInterval;  // set the interval for the timer to count
	REG_TM1CNT = TIMER_ON;  // turn timer 0 back on

	// then start the timer

	// you may have to create additional global variables for these functions to work (to stop the sounds later)
	soundBStart = vblankCounter;
	soundBLooping = isLooping;
	soundB = (char *)sound;
	soundBLength = length;
					// time in seconds == length/frequency
					// time in vblanks == time in seconds * about 60
	soundBDuration = ((length*60) / frequency) - ((length/frequency)*3) - 1;
	soundBFreq = frequency;

	soundBPlaying = 1; // 1 == true
}

void setupInterrupts()
{
	REG_IME = 0;
	REG_INTERRUPT = (unsigned int)interruptHandler;
	// accept interrupts
	REG_IE |= INT_VBLANK;
	// turn on interrupt sending
	REG_DISPSTAT |= INT_VBLANK_ENABLE;

	REG_IME = 1;
}

void interruptHandler()
{
	// turn interrupts off
	REG_IME = 0;

	// check if the interrupt we got was a vblank interrupt
	if(REG_IF & INT_VBLANK)
	{
		// MODIFY THIS FUNCTION !!!
		// Add code to stop/repeat sounds here
		vblankCounter++;
		// stop/repeat sound A
		if((vblankCounter > soundAStart + soundADuration) && soundAPlaying)
		{
			// stop the sound
			dma[1].cnt = 0;
			REG_TM0CNT = 0;
			soundAPlaying = 0;

			// check if we should loop
			if(soundALooping)
			{
				// if we should then play the sound again
				playSoundA(soundA, soundALength, soundAFreq, soundALooping);
			}
		}

		// stop/repeat sound B
		if((vblankCounter > soundBStart + soundBDuration) && soundBPlaying)
		{
			// stop the sound
			dma[2].cnt = 0;
			REG_TM1CNT = 0;
			soundBPlaying = 0;

			// check if we should loop
			if(soundBLooping)
			{
				// if we should then play the sound again
				playSoundB(soundB, soundBLength, soundBFreq, soundBLooping);
			}
		}

		// accept new vblank interrupts
		REG_IF = INT_VBLANK; 
	}

	// turn interrupts back on
	REG_IME = 1;
}
