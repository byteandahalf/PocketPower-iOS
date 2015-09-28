#pragma once
#include "../../../../mcpe/world/level/tile/Tile.h"
class Random;
class TileSource;
struct Player;
class Mob;

class RepeaterTile : public Tile {
public:
	bool powered;
	TextureUVCoordinateSet side_texture;

	RepeaterTile(int, TextureUVCoordinateSet, bool);

	static float torchOffset[4];

	static const TextureUVCoordinateSet& getTexture(RepeaterTile*, signed char, int);
	static void tick(RepeaterTile*, TileSource*, int, int, int, Random*);
	static int getSignal(RepeaterTile*, TileSource*, int, int, int, int);
	static int getDirectSignal(RepeaterTile*, TileSource*, int, int, int, int);
	static void neighborChanged(RepeaterTile*, TileSource*, int, int, int, int, int, int);
	static bool isSignalSource();
	static int getPlacementDataValue(RepeaterTile*, Mob*, int, int, int, signed char);
	static void onPlace(RepeaterTile*, TileSource*, int, int, int);
	static bool use(RepeaterTile*, Player*, int, int, int);
	static int getResource();

private:
	void initVtable();
	bool isGettingPowered(TileSource*, int, int, int, int);
	static int delaySettings[4];
};
