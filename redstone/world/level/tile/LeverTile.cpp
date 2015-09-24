#include "LeverTile.h"
#include "../../../../mcpe/client/renderer/texture/TextureUVCoordinateSet.h"
#include "../../../../mcpe/world/level/TileSource.h"
#include "../../../../mcpe/world/entity/player/Player.h"
#include "../../../../mcpe/world/entity/Mob.h"
#include "../../../../mcpe/world/phys/AABB.h"
#include "../../../../addresses.h"


LeverTile::LeverTile(int blockId) : Tile(blockId, TextureUVCoordinateSet(0.25F, 0.375F, 0.2812F, 0.4375F), tiles[50]->material) {
	init();

	solid[id] = false;
	lightBlock[id] = 0;
	creativeTab = 4;
	renderType = 12;
	destroyTime = 0.5;
	renderPass = 3;

	initVtable();
}

void LeverTile::initVtable() {
	vtable[VT_TILE_GETDIRECT] = (void*) &getDirectSignal;
	vtable[VT_TILE_GETSIGNAL] = (void*) &getSignal;
	vtable[VT_TILE_ONPLACE] = (void*) &onPlace;
	vtable[VT_TILE_MAYPLACE] = (void*) &mayPlace;
	vtable[VT_TILE_SURVIVES] = (void*) &canSurvive;
	vtable[VT_TILE_ISSOURCE] = (void*) &isSignalSource;
	vtable[VT_TILE_NEIGHBORCH] = (void*) &neighborChanged;
	vtable[VT_TILE_ADDCOLLISION] = (void*) &addCollisionShapes;
	vtable[VT_TILE_TICK] = (void*) &tick;
	vtable[VT_TILE_USE] = (void*) &use;
	vtable[VT_TILE_GETSHAPEWORLD] = (void*) &getVisualShapeInWorld;
	vtable[VT_TILE_GETPLACEDATA] = (void*) &getPlacementDataValue;
}

int LeverTile::getPlacementDataValue(LeverTile* self, Mob* placer, int x, int y, int z, signed char side, float xx, float yy, float zz, int ii) {
	int power = placer->region.getData(x, y, z) & 8;
	int sides[6] = {7, 5, 4, 3, 2, 1};
	return sides[side] + power;
}

bool LeverTile::use(LeverTile* self, Player* player, int x, int y, int z) {
	int data = player->region.getData(x, y, z);
	int rot = data & 7;
	int power = 8 - (data & 8);
	player->region.setTileAndData(x, y, z, self->id, rot + power, 3);
	//player->region.fireTilesDirty(x, y, z, x, y, z);
	//ts->getLevel()->playSound((double)i + 0.5D, (double)j + 0.5D, (double)k + 0.5D, "random.click", 0.3F, power <= 0 ? 0.5F : 0.6F);
	player->region.updateNeighborsAt(x, y, z, self->id);
	if(rot == 1) player->region.updateNeighborsAt(x - 1, y, z, self->id);
	else if(rot == 2) player->region.updateNeighborsAt(x + 1, y, z, self->id);
	else if(rot == 3) player->region.updateNeighborsAt(x, y, z - 1, self->id);
	else if(rot == 4) player->region.updateNeighborsAt(x, y, z + 1, self->id);
	else if(rot == 5) player->region.updateNeighborsAt(x, y - 1, z, self->id);
	else if(rot == 7) player->region.updateNeighborsAt(x, y + 1, z, self->id);
	return true;
}

int LeverTile::getDirectSignal(LeverTile* self, TileSource* region, int x, int y, int z, int side) {
	return ((region->getData(x, y, z) & 8) > 0)? 15: 0;
}

int LeverTile::getSignal(LeverTile* self, TileSource* region, int x, int y, int z, int side) {
	int data = region->getData(x, y, z);
	if((data & 8) == 0) return 0;

	int rot = data & 7;
	if(rot == 0 && side == 0) return 15;
	if(rot == 7 && side == 0) return 15;
	if(rot == 6 && side == 1) return 15;
	if(rot == 5 && side == 1) return 15;
	if(rot == 4 && side == 2) return 15;
	if(rot == 3 && side == 3) return 15;
	if(rot == 2 && side == 4) return 15;
	if(rot == 1 && side == 5) return 15;
	return 0;
}

const AABB& LeverTile::getVisualShapeInWorld(LeverTile* self, TileSource* region, int x, int y, int z, AABB& aabb, bool b) {
	float f = 0.25F;
	float f1 = 0.1875F;
	float f2 = 0.1875F;
	switch(region->getData(x, y, z) & 7) {
		case 1:
			aabb.set(0.0F, 0.5F - f, 0.5F - f1, f2, 0.5F + f, 0.5F + f1);
			break;
		case 2:
			aabb.set(1.0F - f2, 0.5F - f, 0.5F - f1, 1.0F, 0.5F + f, 0.5F + f1);
			break;
		case 3:
			aabb.set(0.5F - f1, 0.5F - f, 0.0F, 0.5F + f1, 0.5F + f, f2);
			break;
		case 4:
			aabb.set(0.5F - f1, 0.5F - f, 1.0F - f2, 0.5F + f1, 0.5F + f, 1.0F);
			break;
		case 5:
			aabb.set(0.5F - f1, 0.0F, 0.5F - f, 0.5F + f1, f2, 0.5F + f);
			break;
		case 7:
			aabb.set(0.5F - f1, 1.0F - f2, 0.5F - f, 0.5F + f1, 1.0F, 0.5F + f);
			break;
	}
	return aabb;
}

bool LeverTile::mayPlace(LeverTile* self, TileSource* region, int x, int y, int z, signed char side) {
	switch(side) {
	case 0:
		return solid[region->getTile(x, y + 1, z).id];
	case 1:
		return solid[region->getTile(x, y - 1, z).id];
	case 2:
		return solid[region->getTile(x, y, z + 1).id];
	case 3:
		return solid[region->getTile(x, y, z - 1).id];
	case 4:
		return solid[region->getTile(x + 1, y, z).id];
	case 5:
		return solid[region->getTile(x - 1, y, z).id];
	}
	return true;
}

bool LeverTile::canSurvive(LeverTile* self, TileSource* region, int x, int y, int z) {
	switch(region->getData(x, y, z) & 7) {
	case 7:
		return solid[region->getTile(x, y + 1, z).id];
	case 5:
		return solid[region->getTile(x, y - 1, z).id];
	case 4:
		return solid[region->getTile(x, y, z + 1).id];
	case 3:
		return solid[region->getTile(x, y, z - 1).id];
	case 2:
		return solid[region->getTile(x + 1, y, z).id];
	case 1:
		return solid[region->getTile(x - 1, y, z).id];
	}
	return false;
}

void LeverTile::neighborChanged(LeverTile* self, TileSource* region, int x, int y, int z, int newX, int newY, int newZ) {
	/*if(!canSurvive(region, x, y, z)) {
		region->removeTile(x, y, z);
		region->scheduleBlockUpdate(x, y, z, id, 0);
	}*/
}

void LeverTile::tick(LeverTile* self, TileSource* region, int x, int y, int z, Random* random) {}

void LeverTile::onPlace(LeverTile* self, TileSource* region, int x, int y, int z) {
	region->scheduleBlockUpdate(x, y, z, self->id, 0);
}

bool LeverTile::isSignalSource() {
	return true;
}

void LeverTile::addCollisionShapes() {}