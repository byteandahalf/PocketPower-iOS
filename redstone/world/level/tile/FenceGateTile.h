#pragma once
#include "../../../../mcpe/world/level/tile/Tile.h"

class FenceGateTile : public Tile {
public:
	static void neighborChanged(FenceGateTile*, TileSource*, int, int, int, int, int, int);

	void initVtable();

private:
	static bool isOpen(int);
};