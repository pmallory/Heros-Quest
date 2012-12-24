
//{{BLOCK(victory)

//======================================================================
//
//	victory, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 600 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 38400 + 2048 = 40960
//
//	Time-stamp: 2010-12-05, 22:52:18
//	Exported by Cearn's GBA Image Transmogrifier
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef __VICTORY__
#define __VICTORY__

#define victoryTilesLen 38400
extern const unsigned short victoryTiles[19200];

#define victoryMapLen 2048
extern const unsigned short victoryMap[1024];

#define victoryPalLen 512
extern const unsigned short victoryPal[256];

#endif // __VICTORY__

//}}BLOCK(victory)
