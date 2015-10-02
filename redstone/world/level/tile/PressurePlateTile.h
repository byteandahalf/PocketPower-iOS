#pragma once
#include "../../../../mcpe/world/level/tile/Tile.h"
class Entity;
class TileSource;
class Random;
#include <vector>

class PressurePlateTile : public Tile {
public:
	PressurePlateTile(int, TextureUVCoordinateSet, Material*);

	static void neighborChanged(PressurePlateTile*, TileSource*, int, int, int, int, int, int);
	static bool mayPlace(PressurePlateTile*, TileSource*, int, int, int);
	static bool entityInside(PressurePlateTile*, TileSource*, int, int, int, Entity*);
	static void tick(PressurePlateTile*, TileSource*, int, int, int, Random*);
	static const AABB& getVisualShapeInWorld(PressurePlateTile*, TileSource*, int, int, int, AABB&, bool);
	static int getSignal(PressurePlateTile*, TileSource*, int, int, int, int);
	static int getDirectSignal(PressurePlateTile*, TileSource*, int, int, int, int);
	static void addCollisionShapes();
	static bool isSignalSource();
	static void onRemove(PressurePlateTile*, TileSource*, int, int, int);

	void setStateIfMobInteractsWithPlate(TileSource*, int, int, int, int);
	
protected:
	/*static int getPower(PressurePlateTile*, TileSource*, int, int, int);
	static int getPowerFromData(PressurePlateTile*, int);
	static int getDataFromPower(PressurePlateTile*, int);*/
	static void ABSTRACT_CALL();

	static int getTickDelay();

private:
	void initVtable();
};
