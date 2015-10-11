#include "TrapDoorTile.h"
#include "../../../../mcpe/world/level/TileSource.h"
#include "../../../../addresses.h"

void (*TrapDoorTile::$neighborChanged)(TrapDoorTile*, TileSource*, int, int, int, int, int, int);

void TrapDoorTile::initVtable() {
	$neighborChanged = (void (*)(TrapDoorTile*, TileSource*, int, int, int, int, int, int)) vtable[VT_TILE_NEIGHBORCH];
	vtable[VT_TILE_NEIGHBORCH] = (void*) &neighborChanged;
}

void TrapDoorTile::neighborChanged(TrapDoorTile* self, TileSource* region, int x, int y, int z, int xx, int yy, int zz) {
	$neighborChanged(self, region, x, y, z, xx, yy, zz);

	bool isPowered = region->isBlockIndirectlyGettingPowered(x, y, z);
	Tile* neighbor = tiles[region->getTile(xx, yy, zz).id];
	bool neighborProvidesPower = false;
	
	if(neighbor) {
		bool (*isSignalSource)(Tile*) = (bool (*)(Tile*)) neighbor->vtable[VT_TILE_ISSOURCE];
		neighborProvidesPower = isSignalSource(neighbor);
	}
	
	if(isPowered || neighborProvidesPower || !region->isBlockIndirectlyGettingPowered(xx, yy, zz))
		self->setOpen(region, x, y, z, isPowered);
}

void TrapDoorTile::setOpen(TileSource* region, int x, int y, int z, bool open) {
	int data = region->getData(x, y, z);
	bool isOpen = (data & 4) > 0;

	if(isOpen != open)
		region->setTileAndData(x, y, z, id, data ^ 4, 2);
}