
//{{BLOCK(stats_screen)

//======================================================================
//
//	stats_screen, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 114 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 7296 + 2048 = 9856
//
//	Time-stamp: 2010-12-05, 13:08:58
//	Exported by Cearn's GBA Image Transmogrifier
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef __STATS_SCREEN__
#define __STATS_SCREEN__

#define stats_screenTilesLen 7296
extern const unsigned short stats_screenTiles[3648];

#define stats_screenMapLen 2048
extern const unsigned short stats_screenMap[1024];

#define stats_screenPalLen 512
extern const unsigned short stats_screenPal[256];

#endif // __STATS_SCREEN__

//}}BLOCK(stats_screen)
