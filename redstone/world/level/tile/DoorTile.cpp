#include "DoorTile.h"
#include "../../../../mcpe/world/level/TileSource.h"
#include "../../../../addresses.h"

void (*DoorTile::_neighborChanged)(DoorTile*, TileSource*, int, int, int, int, int, int);
void (*DoorTile::$setOpen)(DoorTile*, TileSource&, int, int, int, bool, Mob*);

void DoorTile::initVtable() {
	_neighborChanged = (void (*)(DoorTile*, TileSource*, int, int, int, int, int, int)) vtable[VT_TILE_NEIGHBORCH];
	vtable[VT_TILE_NEIGHBORCH] = (void*) &neighborChanged;
}

void DoorTile::neighborChanged(DoorTile* self, TileSource* region, int x, int y, int z, int xx, int yy, int zz) {
	_neighborChanged(self, region, x, y, z, xx, yy, zz);
	
	bool powered = region->isBlockIndirectlyGettingPowered(x, y, z);
	if(Tile::tiles[region->getTile(xx, yy, zz).id] != self) {
		self->setOpen(*region, x, y, z, powered, NULL);
	}
}

void DoorTile::setOpen(TileSource& region, int x, int y, int z, bool open, Mob* entity) {
	$setOpen(this, region, x, y, z, open, entity);
}