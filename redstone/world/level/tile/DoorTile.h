#pragma once
#include "../../../../mcpe/world/level/tile/Tile.h"
class Mob;
class TileSource;

class DoorTile : public Tile {
public:
	void initVtable();

	static void (*_neighborChanged)(DoorTile*, TileSource*, int, int, int, int, int, int);
	static void neighborChanged(DoorTile*, TileSource*, int, int, int, int, int, int);
	static void (*$setOpen)(DoorTile*, TileSource&, int, int, int, bool, Mob*);
	
private:
	void setOpen(TileSource&, int, int, int, bool, Mob*);
};
