
//{{BLOCK(battleBG)

//======================================================================
//
//	battleBG, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 170 tiles (t|f reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 10880 + 2048 = 13440
//
//	Time-stamp: 2010-12-05, 14:04:38
//	Exported by Cearn's GBA Image Transmogrifier
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef __BATTLEBG__
#define __BATTLEBG__

#define battleBGTilesLen 10880
extern const unsigned short battleBGTiles[5440];

#define battleBGMapLen 2048
extern const unsigned short battleBGMap[1024];

#define battleBGPalLen 512
extern const unsigned short battleBGPal[256];

#endif // __BATTLEBG__

//}}BLOCK(battleBG)
