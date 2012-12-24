
//{{BLOCK(overworld)

//======================================================================
//
//	overworld, 512x512@8, 
//	+ palette 256 entries, not compressed
//	+ 140 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 64x64 
//	Total size: 512 + 8960 + 8192 = 17664
//
//	Time-stamp: 2010-11-29, 19:49:50
//	Exported by Cearn's GBA Image Transmogrifier
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef __OVERWORLD__
#define __OVERWORLD__

#define overworldTilesLen 8960
extern const unsigned short overworldTiles[4480];

#define overworldMapLen 8192
extern const unsigned short overworldMap[4096];

#define overworldPalLen 512
extern const unsigned short overworldPal[256];

#endif // __OVERWORLD__

//}}BLOCK(overworld)
