#pragma once
#include "PressurePlateTile.h"
class TileSource;

class HeavyPressurePlateTile : public PressurePlateTile {
public:
	HeavyPressurePlateTile(int, TextureUVCoordinateSet, Material*, int);

protected:
	static int getPower(HeavyPressurePlateTile*, TileSource*, int, int, int);
	static int getPowerFromData(HeavyPressurePlateTile*, int);
	static int getDataFromPower(HeavyPressurePlateTile*, int);
	static int getTickDelay();

private:
	int weight;

	void initVtable();
};
