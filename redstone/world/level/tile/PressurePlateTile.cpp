#include "PressurePlateTile.h"
#include "../../../../mcpe/world/level/TileSource.h"
#include "../../../../addresses.h"

PressurePlateTile::PressurePlateTile(int blockId, TextureUVCoordinateSet texture, Material* material) : Tile(blockId, texture, material) {
	init();

	hitbox.set(0.0625F, 0.0F, 0.0625F, 1.0F - 0.0625F, 0.0625F, 1.0F - 0.0625F);

	solid[blockId] = false;
	lightBlock[blockId] = 0;
	destroyTime = 0.5F;
	creativeTab = 4;
	//setTicking(true);
	//setNameId(name);

	initVtable();
}

void PressurePlateTile::initVtable() {
	vtable = (void**) malloc(VT_TILE_SIZE + VT_PLATETILE_SIZEADDR);
	memcpy(vtable, tiles[4]->vtable, VT_TILE_SIZE);

	vtable[VT_TILE_ENTITYINSIDE] = (void*) &entityInside;
	vtable[VT_TILE_MAYPLACE] = (void*) &mayPlace;
	vtable[VT_TILE_NEIGHBORCH] = (void*) &neighborChanged;
	vtable[VT_TILE_TICK] = (void*) &tick;
	vtable[VT_TILE_GETSIGNAL] = (void*) &getSignal;
	vtable[VT_TILE_GETDIRECT] = (void*) &getDirectSignal;
	vtable[VT_TILE_ISSOURCE] = (void*) &isSignalSource;
	vtable[VT_TILE_REMOVE] = (void*) &onRemove;
	vtable[VT_TILE_GETSHAPEWORLD] = (void*) &getVisualShapeInWorld;
	vtable[VT_TILE_ADDCOLLISION] = (void*) &addCollisionShapes;
	vtable[VT_PLATETILE_GETPOWER] = (void*) &ABSTRACT_CALL;
	vtable[VT_PLATETILE_POWERDATA] = (void*) &ABSTRACT_CALL;
	vtable[VT_PLATETILE_DATAPOWER] = (void*) &ABSTRACT_CALL;
	vtable[VT_PLATETILE_TICKDELAY] = (void*) &getTickDelay;
}

bool PressurePlateTile::entityInside(PressurePlateTile* self, TileSource* region, int x, int y, int z, Entity* entity) {
	int (*getPowerFromData)(PressurePlateTile*, int) = (int (*)(PressurePlateTile*, int)) self->vtable[VT_PLATETILE_POWERDATA];
	int power = getPowerFromData(self, region->getData(x, y, z));
	if(power == 0) self->setStateIfMobInteractsWithPlate(region, x, y, z, power);
}

bool PressurePlateTile::mayPlace(PressurePlateTile* self, TileSource* region, int x, int y, int z) {
	int id = region->getTile(x, y - 1, z).id;
	return Tile::solid[id] || id == 89 || id == 85 || id == 113;
}

void PressurePlateTile::neighborChanged(PressurePlateTile* self, TileSource* region, int x, int y, int z, int newX, int newY, int newZ) {
	if(!mayPlace(self, region, x, y, z)) {
		//popResource(region, x, y, z, ItemInstance(id, 1, 0));
		region->setTileAndData(x, y, z, 0, 0, 3);
		region->scheduleBlockUpdate(x, y, z, self->id, 0);
	}
}

void PressurePlateTile::tick(PressurePlateTile* self, TileSource* region, int x, int y, int z, Random* random) {
	int (*getPowerFromData)(PressurePlateTile*, int) = (int (*)(PressurePlateTile*, int)) self->vtable[VT_PLATETILE_POWERDATA];
	int power = getPowerFromData(self, region->getData(x, y, z));
	if(power > 0) self->setStateIfMobInteractsWithPlate(region, x, y, z, power);
}

int PressurePlateTile::getSignal(PressurePlateTile* self, TileSource* region, int x, int y, int z, int side) {
	int (*getPowerFromData)(PressurePlateTile*, int) = (int (*)(PressurePlateTile*, int)) self->vtable[VT_PLATETILE_POWERDATA];
	return (side == 1)? getPowerFromData(self, region->getData(x, y, z)) : 0;
}

int PressurePlateTile::getDirectSignal(PressurePlateTile* self, TileSource* region, int x, int y, int z, int side) {
	int (*getPowerFromData)(PressurePlateTile*, int) = (int (*)(PressurePlateTile*, int)) self->vtable[VT_PLATETILE_POWERDATA];
	return getPowerFromData(self, region->getData(x, y, z));
}

bool PressurePlateTile::isSignalSource() {
	return true;
}

void PressurePlateTile::onRemove(PressurePlateTile* self, TileSource* region, int x, int y, int z) {
	int (*getPowerFromData)(PressurePlateTile*, int) = (int (*)(PressurePlateTile*, int)) self->vtable[VT_PLATETILE_POWERDATA];
	if(getPowerFromData(self, region->getData(x, y, z)) > 0) {
		region->updateNeighborsAt(x, y, z, self->id);
		region->updateNeighborsAt(x, y - 1, z, self->id);
	}
}

void PressurePlateTile::setStateIfMobInteractsWithPlate(TileSource* region, int x, int y, int z, int power) {
	int (*getPower)(PressurePlateTile*, TileSource*, int, int, int) = (int (*)(PressurePlateTile*, TileSource*, int, int, int)) vtable[VT_PLATETILE_GETPOWER];
	int newPower = getPower(this, region, x, y, z);
	bool shouldBePowered = newPower > 0;
	bool isPowered = power > 0;

	if(power != newPower) {
		int (*getDataFromPower)(PressurePlateTile*, int) = (int (*)(PressurePlateTile*, int)) vtable[VT_PLATETILE_DATAPOWER];
		region->setTileAndData(x, y, z, id, getDataFromPower(this, newPower), 2);
		region->updateNeighborsAt(x, y, z, id);
		region->updateNeighborsAt(x, y - 1, z, id);
		//region->fireTilesDirty(x, y, z, x, y, z);
	}

	if(shouldBePowered && !isPowered)
		;//region->getLevel()->playSound(x + 0.5F, y + 0.1F, z + 0.5F, "random.click", 0.3F, 0.6F);
	if(!shouldBePowered && isPowered)
		;//region->getLevel()->playSound(x + 0.5F, y + 0.1F, z + 0.5F, "random.click", 0.3F, 0.5F);

	if(shouldBePowered) {
		int (*getTickDelay)(PressurePlateTile*) = (int (*)(PressurePlateTile*)) vtable[VT_PLATETILE_TICKDELAY];
		region->scheduleBlockUpdate(x, y, z, id, getTickDelay(this));
	}
}

int PressurePlateTile::getTickDelay() {
	return 20;
}

AABB const& PressurePlateTile::getVisualShapeInWorld(PressurePlateTile* self, TileSource* region, int x, int y, int z, AABB& aabb, bool b) {
	aabb.set(0.0625F, 0.0F, 0.0625F, 1.0F - 0.0625F, 0.0625F, 1.0F - 0.0625F);
	int (*getPowerFromData)(PressurePlateTile*, int) = (int (*)(PressurePlateTile*, int)) self->vtable[VT_PLATETILE_POWERDATA];
	if(getPowerFromData(self, region->getData(x, y, z)) == 0)
		aabb.set(0.0625F, 0.0F, 0.0625F, 1.0F - 0.0625F, 0.0625F, 1.0F - 0.0625F);
	else
		aabb.set(0.0625F, 0.0F, 0.0625F, 1.0F - 0.0625F, 0.03125F, 1.0F - 0.0625F);
	return aabb;
}

void PressurePlateTile::addCollisionShapes() {}

void PressurePlateTile::ABSTRACT_CALL() {
	// Simulates an abstract vtable function
	exit(0);
}
