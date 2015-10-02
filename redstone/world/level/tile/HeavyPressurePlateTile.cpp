#include "HeavyPressurePlateTile.h"
#include "../../../../mcpe/world/level/TileSource.h"
#include "../../../../mcpe/world/entity/Entity.h"
#include "../../../../addresses.h"
#include <cmath>
#include <algorithm>

HeavyPressurePlateTile::HeavyPressurePlateTile(int blockId, TextureUVCoordinateSet texture, Material* material, int weight) : PressurePlateTile(blockId, texture, material) {
	this->weight = weight;

	initVtable();
}

void HeavyPressurePlateTile::initVtable() {
	vtable[VT_PLATETILE_GETPOWER] = (void*) &getPower;
	vtable[VT_PLATETILE_POWERDATA] = (void*) &getPowerFromData;
	vtable[VT_PLATETILE_DATAPOWER] = (void*) &getDataFromPower;
	vtable[VT_PLATETILE_TICKDELAY] = (void*) &getTickDelay;
}

int HeavyPressurePlateTile::getDataFromPower(HeavyPressurePlateTile* self, int power) {
	return power;
}

int HeavyPressurePlateTile::getPowerFromData(HeavyPressurePlateTile* self, int data) {
	return data;
}

int HeavyPressurePlateTile::getTickDelay() {
	return 10;
}

int HeavyPressurePlateTile::getPower(HeavyPressurePlateTile* self, TileSource* region, int x, int y, int z) {
	AABB aabb(x + 0.125F, y, z + 0.125F, (x + 1) - 0.125F, y + 0.25F, (z + 1) - 0.125F);
	int smaller = std::min((int) region->getEntities(NULL, aabb).size(), self->weight);

	if(smaller <= 0)
		return 0;
	else {
		float var6 = (float) std::min(self->weight, smaller) / (float) self->weight;
		return ceil(var6 * 15.0F);
	}
}
