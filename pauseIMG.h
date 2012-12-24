
//{{BLOCK(pauseIMG)

//======================================================================
//
//	pauseIMG, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 601 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 38464 + 2048 = 41024
//
//	Time-stamp: 2010-12-06, 00:34:26
//	Exported by Cearn's GBA Image Transmogrifier
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef __PAUSEIMG__
#define __PAUSEIMG__

#define pauseIMGTilesLen 38464
extern const unsigned short pauseIMGTiles[19232];

#define pauseIMGMapLen 2048
extern const unsigned short pauseIMGMap[1024];

#define pauseIMGPalLen 512
extern const unsigned short pauseIMGPal[256];

#endif // __PAUSEIMG__

//}}BLOCK(pauseIMG)
