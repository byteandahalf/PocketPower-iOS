#pragma once

#include "../../../../mcpe/world/level/tile/Tile.h"
struct AABB;
class Random;
struct TextureUVCoordinateSet;
class TileSource;
struct Player;

class ButtonTile : public Tile {
public:
	ButtonTile(int, TextureUVCoordinateSet, Material*);

	bool isWood();
	int getTickDelay();

	static const AABB& getVisualShapeInWorld(ButtonTile*, TileSource*, int, int, int, AABB&, bool);
	static const AABB& getVisualShape(ButtonTile*, unsigned char, AABB&, bool);
	static void onPlace(ButtonTile*, TileSource*, int, int, int);
	static void onRemove(ButtonTile*, TileSource*, int, int, int);
	static void neighborChanged(ButtonTile*, TileSource*, int, int, int, int, int, int);
	static int getPlacementDataValue(ButtonTile*, void*, int, int, int, signed char, float, float, float, int);
	static bool use(ButtonTile*, Player*, int, int, int);
	static void addCollisionShapes();
	static int getSignal(ButtonTile*, TileSource*, int, int, int, int);
	static int getDirectSignal(ButtonTile*, TileSource*, int, int, int, int);
	static void tick(ButtonTile*, TileSource*, int, int, int, Random*);
	static bool isSignalSource();

	void initVtable();
};