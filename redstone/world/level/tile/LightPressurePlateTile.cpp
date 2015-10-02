#include "LightPressurePlateTile.h"
#include "../../../../mcpe/world/level/TileSource.h"
#include "../../../../addresses.h"

LightPressurePlateTile::LightPressurePlateTile(int blockId, TextureUVCoordinateSet texture, Material* material, SENSITIVITY sensitivity) : PressurePlateTile(blockId, texture, material) {
	this->sensitivity = sensitivity;

	initVtable();
}

void LightPressurePlateTile::initVtable() {
	vtable[VT_PLATETILE_GETPOWER] = (void*) &getPower;
	vtable[VT_PLATETILE_POWERDATA] = (void*) &getPowerFromData;
	vtable[VT_PLATETILE_DATAPOWER] = (void*) &getDataFromPower;
}

int LightPressurePlateTile::getDataFromPower(LightPressurePlateTile* self, int power) {
	return power > 0 ? 1 : 0;
}

int LightPressurePlateTile::getPowerFromData(LightPressurePlateTile* self, int data) {
	return data == 1 ? 15 : 0;
}

int LightPressurePlateTile::getPower(LightPressurePlateTile* self, TileSource* region, int x, int y, int z) {
	AABB aabb(x + 0.125F, y, z + 0.125F, (x + 1) - 0.125F, y + 0.25F, (z + 1) - 0.125F);
	EntityList list = region->getEntities(NULL, aabb);

	if(list.size() <= 0)
		return 0;
	if(self->sensitivity == SENSITIVITY::EVERYTHING || (self->sensitivity == SENSITIVITY::MOBS && self->_listIncludesMob(list)))
		return 15;
	
	return 0;
}

bool LightPressurePlateTile::_EntityisMob(const Entity* entity) {
	int (*getEntityTypeId)(Entity*) = (int (*)(Entity*)) entity->vtable[85];
	int id = getEntityTypeId(entity);
	return id != 64 && id != 80 && id != 81 && id != 82 && id != 66;
}

bool LightPressurePlateTile::_listIncludesMob(EntityList& list) {
	for(int i = 0; i < list.size(); i++) {
		if(_EntityisMob(list[i])) return true;
	}
	return false;
}
