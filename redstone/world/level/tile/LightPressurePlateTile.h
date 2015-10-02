#pragma once
#include "PressurePlateTile.h"
#include "../../../../mcpe/world/entity/Entity.h"

class LightPressurePlateTile : public PressurePlateTile {
public:
	enum class SENSITIVITY {
		EVERYTHING,
		MOBS
	};

	LightPressurePlateTile(int, TextureUVCoordinateSet, Material*, SENSITIVITY);

protected:
	static int getPower(LightPressurePlateTile*, TileSource*, int, int, int);
	static int getPowerFromData(LightPressurePlateTile*, int);
	static int getDataFromPower(LightPressurePlateTile*, int);

private:
	SENSITIVITY sensitivity;

	bool _listIncludesMob(EntityList&);
	bool _EntityisMob(Entity*);

	void initVtable();
};
