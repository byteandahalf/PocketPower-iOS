#pragma once
#include "../../../../mcpe/world/level/tile/Tile.h"
class TileSource;
class Mob;

class PistonBaseTile: public Tile {
public:
	PistonBaseTile(int, bool);

	static const TextureUVCoordinateSet& getTexture(PistonBaseTile*, signed char, int);
	static const TextureUVCoordinateSet& getTextureInWorld(PistonBaseTile*, TileSource*, int, int, int, signed char);
	static void onPlace(PistonBaseTile*, TileSource*, int, int, int);
	static const AABB& getVisualShapeInWorld(PistonBaseTile*, TileSource*, int, int, int, AABB&, bool);
	static const AABB& getVisualShape(PistonBaseTile*, unsigned char, AABB&, bool);
	static int getPlacementDataValue(PistonBaseTile*, Mob*, int, int, int, signed char);
	//static void addCollisionShapes(PistonBaseTile*, TileSource&, int, int, int, const AABB*, std::vector<AABB, std::allocator<AABB>>&);
	static void triggerEvent(PistonBaseTile*, TileSource*, int, int, int, int, int);
	static void neighborChanged(PistonBaseTile*, TileSource*, int, int, int, int, int, int);

	static int getRotation(int);
	static bool isPowered(int);

private:
	TextureUVCoordinateSet texture_inner;
	TextureUVCoordinateSet texture_bottom;
	TextureUVCoordinateSet texture_side;
	bool sticky;

	static bool isMoveableBlock(Tile*, TileSource*, int, int, int, bool);
	static bool canPushRow(TileSource*, int, int, int, int);
	bool actuallyPushRow(TileSource*, int, int, int, int);
	bool hasPower(TileSource*, int, int, int, int);
	void updateState(TileSource*, int, int, int);

	void initVtable();
};
