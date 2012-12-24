
//{{BLOCK(splash)

//======================================================================
//
//	splash, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 600 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 38400 + 2048 = 40960
//
//	Time-stamp: 2010-11-29, 21:27:07
//	Exported by Cearn's GBA Image Transmogrifier
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef __SPLASH__
#define __SPLASH__

#define splashTilesLen 38400
extern const unsigned short splashTiles[19200];

#define splashMapLen 2048
extern const unsigned short splashMap[1024];

#define splashPalLen 512
extern const unsigned short splashPal[256];

#endif // __SPLASH__

//}}BLOCK(splash)
