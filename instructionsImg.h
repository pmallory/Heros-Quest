
//{{BLOCK(instructionsImg)

//======================================================================
//
//	instructionsImg, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 333 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 21312 + 2048 = 23872
//
//	Time-stamp: 2010-12-06, 01:03:02
//	Exported by Cearn's GBA Image Transmogrifier
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef __INSTRUCTIONSIMG__
#define __INSTRUCTIONSIMG__

#define instructionsImgTilesLen 21312
extern const unsigned short instructionsImgTiles[10656];

#define instructionsImgMapLen 2048
extern const unsigned short instructionsImgMap[1024];

#define instructionsImgPalLen 512
extern const unsigned short instructionsImgPal[256];

#endif // __INSTRUCTIONSIMG__

//}}BLOCK(instructionsImg)
