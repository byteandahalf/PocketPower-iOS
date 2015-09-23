#pragma once
#include <Forklift/Forklift.h>
#include "../../../world/phys/AABB.h"
struct TilePos;
class RedstoneTile;
class NotGateTile;
class Tile;
class Tessellator;
class TileSource;

#define TILETESS_FILLER FLArch(668, 668, 1192)

class TileTessellator {
public:
	void** vtable;
	TileSource* region;
	char filler2[TILETESS_FILLER];
	Tessellator* tessellator;
	AABB bounds;

	bool tessellateRedstoneInWorld(RedstoneTile*, const TilePos&, int);
	bool tessellateLitNotGateInWorld(NotGateTile*, const TilePos&, int);
	void tessellateAngledNotGate(Tile*, double, double, double, double, double);
	bool tessellateBlockInWorld(Tile*, const TilePos&);

	static bool (*$tessellateBlockInWorld)(TileTessellator*, Tile*, const TilePos&);
};