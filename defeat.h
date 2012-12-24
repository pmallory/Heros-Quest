
//{{BLOCK(defeat)

//======================================================================
//
//	defeat, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 601 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 38464 + 2048 = 41024
//
//	Time-stamp: 2010-12-05, 23:17:26
//	Exported by Cearn's GBA Image Transmogrifier
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef __DEFEAT__
#define __DEFEAT__

#define defeatTilesLen 38464
extern const unsigned short defeatTiles[19232];

#define defeatMapLen 2048
extern const unsigned short defeatMap[1024];

#define defeatPalLen 512
extern const unsigned short defeatPal[256];

#endif // __DEFEAT__

//}}BLOCK(defeat)
