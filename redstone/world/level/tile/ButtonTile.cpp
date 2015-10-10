#include "ButtonTile.h"
#include "../../../../addresses.h"
#include "../../../../mcpe/world/level/TileSource.h"
#include "../../../../mcpe/world/entity/player/Player.h"
#include "../../../../mcpe/world/phys/AABB.h"
class Random;


ButtonTile::ButtonTile(int id, TextureUVCoordinateSet texture, Material* material) : Tile(id, texture, material) {
	init();

	Tile::solid[id] = false;
	Tile::lightBlock[id] = 0;
	ticks = true;
	destroyTime = 0.5;
	creativeTab = 4;

	initVtable();
}

void ButtonTile::initVtable() {
	vtable[VT_TILE_GETDIRECT] = (void*) &getDirectSignal;
	vtable[VT_TILE_GETSIGNAL] = (void*) &getSignal;
	vtable[VT_TILE_ONPLACE] = (void*) &onPlace;
	vtable[VT_TILE_REMOVE] = (void*) &onRemove;
	vtable[VT_TILE_ISSOURCE] = (void*) &isSignalSource;
	vtable[VT_TILE_NEIGHBORCH] = (void*) &neighborChanged;
	vtable[VT_TILE_ADDCOLLISION] = (void*) &addCollisionShapes;
	vtable[VT_TILE_TICK] = (void*) &tick;
	vtable[VT_TILE_USE] = (void*) &use;
	vtable[VT_TILE_GETSHAPE] = (void*) &getVisualShape;
	vtable[VT_TILE_GETSHAPEWORLD] = (void*) &getVisualShapeInWorld;
	vtable[VT_TILE_GETPLACEDATA] = (void*) &getPlacementDataValue;
	vtable[VT_TILE_SURVIVES] = (void*) &canSurvive;
}

bool ButtonTile::canSurvive(ButtonTile* self, TileSource* region, int x, int y, int z) {
    int rot = region->getData(x, y, z) & 7;
    if(rot == 6)
	return solid[region->getTile(x, y + 1, z).id];
    if(rot == 5)
	return solid[region->getTile(x, y - 1, z).id];
    if(rot == 4)
	return solid[region->getTile(x, y, z + 1).id];
    if(rot == 3)
	return solid[region->getTile(x, y, z - 1).id];
    if(rot == 2)
	return solid[region->getTile(x + 1, y, z).id];
    return rot == 1 && solid[region->getTile(x - 1, y, z).id];
}

int ButtonTile::getTickDelay() {
	return (isWood())? 30 : 20;
}

void ButtonTile::onPlace(ButtonTile* self, TileSource* region, int x, int y, int z) {
	region->scheduleBlockUpdate(x, y, z, self->id, 0);
}

int ButtonTile::getPlacementDataValue(ButtonTile* self, Mob* placer, int x, int y, int z, signed char side, float xx, float yy, float zz, int ii) {
	switch(side) {
		case 0: return 6;
		case 1: return 5;
		case 2: return 4;
		case 3: return 3;
		case 4: return 2;
		case 5: return 1;
		default: return 4;
	}
}

const AABB& ButtonTile::getVisualShape(ButtonTile* self, unsigned char data, AABB& aabb, bool b) {
	float f = 0.1875F;
	float f1 = 0.125F;
	float f2 = 0.125F;
	aabb.set(0.5F - f, 0.5F - f1, 0.5F - f2, 0.5F + f, 0.5F + f1, 0.5F + f2);
	return aabb;
}

const AABB& ButtonTile::getVisualShapeInWorld(ButtonTile* self, TileSource* region, int x, int y, int z, AABB& aabb, bool b) {
	int data = region->getData(x, y, z);
	int rot = data & 7;
	bool powered = (data & 8) > 0;
	float f = 0.375F;
	float f1 = 0.625F;
	float f2 = 0.1875F;
	float f3 = 0.125F;
	if(powered) f3 = 0.0625F;
	if(rot == 1)
		aabb.set(0.0F, f, 0.5F - f2, f3, f1, 0.5F + f2);
	else if(rot == 2)
		aabb.set(1.0F - f3, f, 0.5F - f2, 1.0F, f1, 0.5F + f2);
	else if(rot == 3)
		aabb.set(0.5F - f2, f, 0.0F, 0.5F + f2, f1, f3);
	else if(rot == 4)
		aabb.set(0.5F - f2, f, 1.0F - f3, 0.5F + f2, f1, 1.0F);
	else if(rot == 5)
		aabb.set(f, 0.0F, 0.5F - f2, f1, f3, 0.5F + f2);
	else if(rot == 6)
		aabb.set(f, 1.0F - f3, 0.5F - f2, f1, 1.0F, 0.5F + f2);

	return aabb;
}

void ButtonTile::neighborChanged(ButtonTile* self, TileSource* region, int x, int y, int z, int newX, int newY, int newZ) {
	if(!canSurvive(self, region, x, y, z)) {
		region->setTileAndData(x, y, z, 0, 0, 3);
		popResource(region, x, y, z, ItemInstance(self->id, 1, 0));
		region->scheduleBlockUpdate(x, y, z, self->id, 0);
	}
}

bool ButtonTile::use(ButtonTile* self, Player* player, int x, int y, int z) {
	int data = player->region.getData(x, y, z);
	int rot = data & 7;
	int power = 8 - (data & 8);
	if(power == 0) return true;
	player->region.setTileAndData(x, y, z, self->id, rot + power, 3);
	//player->region.fireTilesDirty(x, y, z, x, y, z);
	//ts->getLevel()->playSound(x + 0.5F, y + 0.5F, z + 0.5F, "random.click", 0.3F, 0.6F);
	player->region.updateNeighborsAt(x, y, z, self->id);
	if(rot == 1)
		player->region.updateNeighborsAt(x - 1, y, z, self->id);
	else if(rot == 2)
		player->region.updateNeighborsAt(x + 1, y, z, self->id);
	else if(rot == 3)
		player->region.updateNeighborsAt(x, y, z - 1, self->id);
	else if(rot == 4)
		player->region.updateNeighborsAt(x, y, z + 1, self->id);
	else if(rot == 5)
		player->region.updateNeighborsAt(x, y - 1, z, self->id);
	else if(rot == 6)
		player->region.updateNeighborsAt(x, y + 1, z, self->id);
	player->region.scheduleBlockUpdate(x, y, z, self->id, self->getTickDelay());
	return true;
}

void ButtonTile::onRemove(ButtonTile* self, TileSource* region, int x, int y, int z) {
	int data = region->getData(x, y, z);
	if((data & 8) > 0) {
		region->updateNeighborsAt(x, y, z, self->id);

		switch(data & 7) {
		case 6:
			region->updateNeighborsAt(x, y + 1, z, self->id);
			break;
		case 5:
			region->updateNeighborsAt(x, y - 1, z, self->id);
			break;
		case 4:
			region->updateNeighborsAt(x, y, z + 1, self->id);
			break;
		case 3:
			region->updateNeighborsAt(x, y, z - 1, self->id);
			break;
		case 2:
			region->updateNeighborsAt(x + 1, y, z, self->id);
			break;
		case 1:
			region->updateNeighborsAt(x - 1, y, z, self->id);
			break;
		}
	}
	//Tile::onRemove(region, x, y, z);
}

int ButtonTile::getDirectSignal(ButtonTile* self, TileSource* region, int x, int y, int z, int side) {
	return ((region->getData(x, y, z) & 8) > 0)? 15: 0;
}

int ButtonTile::getSignal(ButtonTile* self, TileSource* region, int x, int y, int z, int side) {
	int data = region->getData(x, y, z);
	if((data & 8) == 0) return 0;
	int rot = data & 7;
	if(rot == 6 && side == 0)
		return 15;
	if(rot == 5 && side == 1)
		return 15;
	if(rot == 4 && side == 2)
		return 15;
	if(rot == 3 && side == 3)
		return 15;
	if(rot == 2 && side == 4)
		return 15;
	if(rot == 1 && side == 5)
		return 15;
	return 0;
}
	
bool ButtonTile::isSignalSource() {
	return true;
}

void ButtonTile::tick(ButtonTile* self, TileSource* region, int x, int y, int z, Random* random) {
	int data = region->getData(x, y, z);

	region->setTileAndData(x, y, z, self->id, data & 7, 3);
	region->updateNeighborsAt(x, y, z, self->id);
	int rot = data & 7;
	if(rot == 1)
		region->updateNeighborsAt(x - 1, y, z, self->id);
	else if(rot == 2)
		region->updateNeighborsAt(x + 1, y, z, self->id);
	else if(rot == 3)
		region->updateNeighborsAt(x, y, z - 1, self->id);
	else if(rot == 4)
		region->updateNeighborsAt(x, y, z + 1, self->id);
	else if(rot == 5)
		region->updateNeighborsAt(x, y - 1, z, self->id);
	else if(rot == 6)
		region->updateNeighborsAt(x, y + 1, z, self->id);
	//ts->getLevel()->playSound(x + 0.5F, y + 0.5F, z + 0.5F, "random.click", 0.3F, 0.5F);
	//region->fireTilesDirty(x, y, z, x, y, z);
}

bool ButtonTile::isWood() {
	return id == 143;
}

void ButtonTile::addCollisionShapes() {}