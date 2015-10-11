#include "FenceGateTile.h"
#include "../../../../mcpe/world/level/TileSource.h"
#include "../../../../mcpe/world/level/TilePos.h"
#include "../../../../addresses.h"

void FenceGateTile::initVtable() {
	vtable[VT_TILE_NEIGHBORCH] = (void*) &neighborChanged;
}

void FenceGateTile::neighborChanged(FenceGateTile* self, TileSource* region, int x, int y, int z, int xx, int yy, int zz) {
	int data = region->getData(x, y, z);
	bool powered = region->isBlockIndirectlyGettingPowered(x, y, z);

	if(powered && !isOpen(data)) {
		region->setTileAndData(x, y, z, self->id, data | 4, 2);
		//region->getLevel()->playSound(x + 0.5F, y + 0.5F, z + 0.5F, "random.door_open", 0.3F, 1.0F);
	}
	if(!powered && isOpen(data)) {
		region->setTileAndData(x, y, z, self->id, data & -5, 2);
		//region->getLevel()->playSound(x + 0.5F, y + 0.5F, z + 0.5F, "random.door_close", 0.3F, 1.0F);
	}
}

bool FenceGateTile::isOpen(int) {
	return (data & 4) > 0;
}