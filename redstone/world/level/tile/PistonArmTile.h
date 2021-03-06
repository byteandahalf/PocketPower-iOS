#pragma once
#include "../../../../mcpe/world/level/tile/Tile.h"
#include <vector>
class TileSource;
class AABB;
class Entity;

class PistonArmTile : public Tile {
public:
	PistonArmTile(int);
	
	static const AABB& getVisualShapeInWorld(PistonArmTile*, TileSource*, int, int, int, AABB&, bool);
	static void neighborChanged(PistonArmTile*, TileSource*, int, int, int, int, int, int);
	static void destroy(PistonArmTile*, TileSource*, int, int, int, int, Entity*);
	static const TextureUVCoordinateSet& getTexture(PistonArmTile*, signed char, int);
	static bool addCollisionShapes(PistonArmTile*, TileSource&, int, int, int, AABB const*, std::vector<AABB>&);
	
	static int getRotation(int);
	static bool isSticky(int);
	
	TextureUVCoordinateSet texture_sticky;
	TextureUVCoordinateSet texture_side;

private:
	void initVtable();
};
