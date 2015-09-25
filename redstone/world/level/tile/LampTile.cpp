#include "LampTile.h"

#include "../../../../mcpe/world/level/TileSource.h"
#include "../../../../addresses.h"

LampTile::LampTile(int blockId, TextureUVCoordinateSet texture) : Tile(blockId, texture, tiles[1]->material) {
	init();

	Tile* (*setLightEmission)(Tile*, float) = (Tile* (*)(Tile*, float)) vtable[VT_TILE_SETEMISSION];
	if(isLit()) setLightEmission(this, 1.0F);
	creativeTab = 4;
	//setNameId("redstoneLight");
	destroyTime = 0.3F;
	soundType = tiles[20]->soundType;

	initVtable();
}

void LampTile::initVtable() {
	vtable[VT_TILE_ONPLACE] = (void*) &onPlace;
	vtable[VT_TILE_NEIGHBORCH] = (void*) &neighborChanged;
	vtable[VT_TILE_TICK] = (void*) &tick;
	vtable[VT_TILE_RESOURCE] = (void*) &getResource;
}

void LampTile::onPlace(LampTile* self, TileSource* region, int x, int y, int z) {
	if(self->isLit() && !region->isBlockIndirectlyGettingPowered(x, y, z))
		region->scheduleBlockUpdate(x, y, z, self->id, 4);
	else if(!self->isLit() && region->isBlockIndirectlyGettingPowered(x, y, z))
		region->setTileAndData(x, y, z, 124, 0, 3);
}

void LampTile::neighborChanged(LampTile* self, TileSource* region, int x, int y, int z, int newX, int newY, int newZ) {
	if(self->isLit() && !region->isBlockIndirectlyGettingPowered(x, y, z))
		region->scheduleBlockUpdate(x, y, z, self->id, 4);
	else if(!self->isLit() && region->isBlockIndirectlyGettingPowered(x, y, z))
		region->setTileAndData(x, y, z, 124, 0, 3);
}

void LampTile::tick(LampTile* self, TileSource* region, int x, int y, int z, Random* random) {
	if(self->isLit() && !region->isBlockIndirectlyGettingPowered(x, y, z))
		region->setTileAndData(x, y, z, 123, 0, 3);
}

int LampTile::getResource(LampTile* self, int id, Random* random) {
	return 123;
}

bool LampTile::isLit() {
	return id == 124;
}