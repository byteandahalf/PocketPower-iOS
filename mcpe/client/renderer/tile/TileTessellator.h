#pragma once
#include <Forklift/Forklift.h>
#include "../../../world/phys/AABB.h"
#include "../texture/TextureUVCoordinateSet.h"
struct TilePos;
class RedstoneTile;
class NotGateTile;
class LeverTile;
class RepeaterTile;
class PistonBaseTile;
class PistonArmTile;
class Tile;
class Tessellator;
class TileSource;

#define TILETESS_FILLER FLArch(520, 1044)

class TileTessellator {
public:
	void** vtable;
	TileSource* region;
	TextureUVCoordinateSet forcedUV;
	bool useForcedUV;
	char filler[88];
	int rotBottom;
	int rotTop;
	int rotEast;
	int rotWest;
	int rotNorth;
	int rotSouth;
	char filler2[TILETESS_FILLER];
	Tessellator* tessellator;
	AABB bounds;

	bool tessellateRedstoneInWorld(RedstoneTile*, const TilePos&, int);
	bool tessellateLeverInWorld(LeverTile*, const TilePos&);
	bool tessellateLitNotGateInWorld(NotGateTile*, const TilePos&, int);
	void tessellateAngledNotGate(Tile*, double, double, double, double, double);
	bool tessellateBlockInWorld(Tile*, const TilePos&);
	bool tessellateRepeaterInWorld(RepeaterTile*, const TilePos&, int);
	bool tessellatePistonBaseInWorld(PistonBaseTile*, const TilePos&, bool);
	bool tessellatePistonArmInWorld(PistonArmTile*, const TilePos&, bool);
	void tessellatePistonRodUD(PistonArmTile*, float, float, float, float, float, float, float, float);
	void tessellatePistonRodSN(PistonArmTile*, float, float, float, float, float, float, float, float);
	void tessellatePistonRodEW(PistonArmTile*, float, float, float, float, float, float, float, float);

	static bool (*$tessellateBlockInWorld)(TileTessellator*, Tile*, const TilePos&);
};