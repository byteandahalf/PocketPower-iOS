#include "TntTile.h"
#include "../TileSource.h"
#include "../../../../addresses.h"


void TntTile::initVtable(TntTile* self) {
	solid[self->id] = false;
	lightBlock[self->id] = 0;
	self->vtable[VT_TILE_ONPLACE] = (void*) &onPlace;
	self->vtable[VT_TILE_NEIGHBORCH] = (void*) &neighborChanged;
}

void TntTile::onPlace(TntTile* self, TileSource* region, int x, int y, int z) {
	neighborChanged(self, region, x, y, z, x, y, z);
}

void TntTile::neighborChanged(TntTile* self, TileSource* region, int x, int y, int z, int newX, int newY, int newZ) {
	if(region->isBlockIndirectlyGettingPowered(x, y, z)) {
		void (*destroy)(TntTile*, TileSource*, int, int, int, int, void*) = (void (*)(TntTile*, TileSource*, int, int, int, int, void*)) self->vtable[VT_TILE_DESTROY];
		destroy(self, region, x, y, z, 1, NULL);
		region->setTileAndData(x, y, z, 0, 0, 3);
	}
}