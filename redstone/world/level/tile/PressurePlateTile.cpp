#include "PressurePlateTile.h"
#include "../../../../mcpe/world/level/TileSource.h"
#include "../../../../mcpe/world/phys/AABB.h"
#include "../../../../addresses.h"


PressurePlateTile::PressurePlateTile(int id, TextureUVCoordinateSet texture, Material* material) : Tile(id, texture, material) {
	init();
	
	hitbox.set(0.0625F, 0.0F, 0.0625F, 1.0F - 0.0625F, 0.0625F, 1.0F - 0.0625F);
	solid[id] = false;
	lightBlock[id] = 0;
	shouldTick[id] = true;
	destroyTime = 1.0;
	creativeTab = 4;

	initVtable();
}

void PressurePlateTile::initVtable() {
	vtable[VT_TILE_ENTITYINSIDE] = (void*) &entityInside;
	vtable[VT_TILE_TICK] = (void*) &tick;
	vtable[VT_TILE_GETSIGNAL] = (void*) &getSignal;
	vtable[VT_TILE_GETDIRECT] = (void*) &getDirectSignal;
	vtable[VT_TILE_ISSOURCE] = (void*) &isSignalSource;
	vtable[VT_TILE_REMOVE] = (void*) &onRemove;
	vtable[VT_TILE_ONPLACE] = (void*) &onPlace;
	vtable[VT_TILE_GETSHAPE] = (void*) &getVisualShape;
	vtable[VT_TILE_GETSHAPEWORLD] = (void*) &getVisualShapeInWorld;
	vtable[VT_TILE_ADDCOLLISION] = (void*) &addCollisionShapes;
	vtable[VT_TILE_MAYPLACE] = (void*) &mayPlace;

}

void PressurePlateTile::entityInside(PressurePlateTile* self, TileSource* region, int x, int y, int z, Entity* entity) {
	if(region->getData(x, y, z) != 1) self->setStateIfMobInteractsWithPlate(region, x, y, z);
}

bool PressurePlateTile::mayPlace(PressurePlateTile* self, TileSource* region, int x, int y, int z) {
	int id = region->getTile(x, y - 1, z).id;
	return solid[id] || id == 89;
}

void PressurePlateTile::neighborChanged(PressurePlateTile* self, TileSource* region, int x, int y, int z, int newX, int newY, int newZ) {
	//if(!mayPlace(region, x, y, z)) {
		//popResource(region, x, y, z, ItemInstance(id, 1, 0));
	//	region->removeTile(x, y, z);
	//	region->scheduleBlockUpdate(x, y, z, id, 0);
	//}
}

void PressurePlateTile::tick(PressurePlateTile* self, TileSource* region, int x, int y, int z, Random* random) {
	if(region->getData(x, y, z) != 0) self->setStateIfMobInteractsWithPlate(region, x, y, z);
}

int PressurePlateTile::getSignal(PressurePlateTile* self, TileSource* region, int x, int y, int z, int side) {
	if(region->getData(x, y, z) == 0) return 0; 
	if(side == 1) return 15;
	return 0;
}

int PressurePlateTile::getDirectSignal(PressurePlateTile* self, TileSource* region, int x, int y, int z, int side) {
	return (region->getData(x, y, z) > 0)? 15 : 0;
}

bool PressurePlateTile::isSignalSource(PressurePlateTile* self) {
	return true;
}

void PressurePlateTile::onRemove(PressurePlateTile* self, TileSource* region, int x, int y, int z) {
	if(region->getData(x, y, z) > 0) {
		region->updateNeighborsAt(x, y, z, self->id);
		region->updateNeighborsAt(x, y - 1, z, self->id);
	}
	//Tile::onRemove(region, x, y, z);
}

bool PressurePlateTile::_isMob(Entity* entity) {
	int (*getId)(Entity*) = (int (*)(Entity*)) entity->vtable[85];
	int id = getId(entity);
	return id != 64 && id != 80 && id != 81 && id != 82 && id != 66;
}

bool PressurePlateTile::_listContainsMob(EntityList& list) {
	for(int i = 0; i < (int) list.size(); i++) {
		if(_isMob(list[i])) return true;
	}
	return false;
}

void PressurePlateTile::setStateIfMobInteractsWithPlate(TileSource* region, int x, int y, int z) {
	bool isPowered = region->getData(x, y, z) == 1;
	bool shouldBePowered = false;
	AABB aabb(x + 0.125F, y, z + 0.125F, (x + 1) - 0.125F, y + 0.25F, (z + 1) - 0.125F);
	EntityList list = region->getEntities(NULL, aabb);

	if(list.size() > 0) {
		if(isWood() || (!isWood() && _listContainsMob(list)))
			shouldBePowered = true;
	}
	if(shouldBePowered && !isPowered) {
		region->setTileAndData(x, y, z, id, 1, 3);
		region->updateNeighborsAt(x, y, z, id);
		region->updateNeighborsAt(x, y - 1, z, id);
		//region->fireTilesDirty(x, y, z, x, y, z);
		//region->getLevel()->playSound(x + 0.5F, y + 0.10000000000000001F, z + 0.5F, "random.click", 0.3F, 0.6F);
	}

	if(!shouldBePowered && isPowered) {
		region->setTileAndData(x, y, z, id, 0, 3);
		region->updateNeighborsAt(x, y, z, id);
		region->updateNeighborsAt(x, y - 1, z, id);
		//region->fireTilesDirty(x, y, z, x, y, z);
		//region->getLevel()->playSound(x + 0.5F, y + 0.1F, z + 0.5F, "random.click", 0.3F, 0.5F);
	}

	if(shouldBePowered) region->scheduleBlockUpdate(x, y, z, id, getTickDelay());
}

int PressurePlateTile::getTickDelay() {
	return 20;
}

const AABB& PressurePlateTile::getVisualShapeInWorld(PressurePlateTile* self, TileSource* region, int x, int y, int z, AABB& aabb, bool b) {
	aabb.set(0.0625F, 0.0F, 0.0625F, 1.0F - 0.0625F, 0.0625F, 1.0F - 0.0625F);
	if(region->getData(x, y, z) != 0)
		aabb.set(0.0625F, 0.0F, 0.0625F, 1.0F - 0.0625F, 0.03125F, 1.0F - 0.0625F);

	return aabb;
}

const AABB& PressurePlateTile::getVisualShape(PressurePlateTile* self, unsigned char data, AABB& aabb, bool b) {
	aabb.set(0.0F, 0.5F - 0.125F, 0.0F, 1.0F, 0.5F + 0.125F, 1.0F);
	return aabb;
}

bool PressurePlateTile::isWood() {
	return id == 72;
}

void PressurePlateTile::onPlace(PressurePlateTile* self, TileSource* region, int x, int y, int z) {}
void PressurePlateTile::addCollisionShapes(PressurePlateTile* self) {}