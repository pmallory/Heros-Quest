
//{{BLOCK(overworld_map)

//======================================================================
//
//	overworld_map, 512x512@8, 
//	+ palette 256 entries, not compressed
//	+ 193 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 64x64 
//	Total size: 512 + 12352 + 8192 = 21056
//
//	Time-stamp: 2010-11-29, 19:54:11
//	Exported by Cearn's GBA Image Transmogrifier
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef __OVERWORLD_MAP__
#define __OVERWORLD_MAP__

#define overworld_mapTilesLen 12352
extern const unsigned short overworld_mapTiles[6176];

#define overworld_mapMapLen 8192
extern const unsigned short overworld_mapMap[4096];

#define overworld_mapPalLen 512
extern const unsigned short overworld_mapPal[256];

#endif // __OVERWORLD_MAP__

//}}BLOCK(overworld_map)
