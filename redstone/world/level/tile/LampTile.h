#pragma once

#include "../../../../mcpe/world/level/tile/Tile.h"
class Random;
class TileSource;

class LampTile : public Tile {
public:
	LampTile(int, TextureUVCoordinateSet);

	static void onPlace(LampTile*, TileSource*, int, int, int);
	static void neighborChanged(LampTile*, TileSource*, int, int, int, int, int, int);
	static void tick(LampTile*, TileSource*, int, int, int, Random*);
	static int getResource(LampTile*, int, Random*);

	bool isLit();

	void initVtable();
};