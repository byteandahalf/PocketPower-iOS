#pragma once
#include "../../../../mcpe/world/level/tile/Tile.h"
class TileSource;

class TrapDoorTile : public Tile {
public:
	static void neighborChanged(TrapDoorTile*, TileSource*, int, int, int, int, int, int);
	static void (*$neighborChanged)(TrapDoorTile*, TileSource*, int, int, int, int, int, int);

	void initVtable();

private:
	void setOpen(TrapDoorTile*, TileSource*, int, int, int, bool);
};
