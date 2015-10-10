#include "NotGateTile.h"
#include "../../../../addresses.h"
#include "../../../../mcpe/world/level/TileSource.h"
#include "../../../../mcpe/world/item/ItemInstance.h"

NotGateTile::NotGateTile(int id, TextureUVCoordinateSet texture) : Tile(id, texture, tiles[50]->material) {
    init();

    memcpy(vtable, tiles[50]->vtable, VT_TILE_SIZE);

    if(isActive()) {
	renderType = 100;
	renderPass = 7;
    } else {
	renderType = 2;
    }

    hitbox.set(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);
    destroyTime = 0.0F;
    solid[id] = false;
    lightBlock[id] = 0;
    creativeTab = 4;

    initVtable();
}

void NotGateTile::initVtable() {
    vtable[VT_TILE_GETDIRECT] = (void*) &getDirectSignal;
    vtable[VT_TILE_GETSIGNAL] = (void*) &getSignal;
    vtable[VT_TILE_ONPLACE] = (void*) &onPlace;
    vtable[VT_TILE_REMOVE] = (void*) &onRemove;
    vtable[VT_TILE_ISSOURCE] = (void*) &isSignalSource;
    vtable[VT_TILE_NEIGHBORCH] = (void*) &neighborChanged;
    vtable[VT_TILE_ADDCOLLISION] = (void*) &addCollisionShapes;
    vtable[VT_TILE_TICK] = (void*) &tick;
    vtable[VT_TILE_ANIMATE] = (void*) &addCollisionShapes;
    vtable[VT_TILE_SURVIVES] = (void*) &canSurvive;
    vtable[VT_TILE_RESOURCE] = (void*) &getResource;
}

bool NotGateTile::isActive() {
    return id == 76;
}

int NotGateTile::getDirectSignal(NotGateTile* tile, TileSource* region, int x, int y, int z, int side) {
    if(!tile->isActive()) return 0;

    int data = region->getData(x, y, z);
    if(data == 5 && side == 1) return 0;
    if(data == 3 && side == 3) return 0;
    if(data == 4 && side == 2) return 0;
    if(data == 1 && side == 5) return 0;
    if(data == 2 && side == 4) return 0;
    return 15;
}

bool NotGateTile::canSurvive(NotGateTile* self, TileSource* region, int x, int y, int z) {
    int data = region->getData(x, y, z);
    if((data == 5 || data == 0) && solid[region->getTile(x, y - 1, z).id]) return true;
    if(data == 3 && solid[region->getTile(x, y, z - 1).id]) return true;
    if(data == 4 && solid[region->getTile(x, y, z + 1).id]) return true;
    if(data == 1 && solid[region->getTile(x - 1, y, z).id]) return true;
    return data == 2 && solid[region->getTile(x + 1, y, z).id];
}

int NotGateTile::getSignal(NotGateTile* tile, TileSource* region, int x, int y, int z, int side) {
    if(side == 0) return getDirectSignal(tile, region, x, y, z, side);
    return 0;
}

void NotGateTile::onPlace(NotGateTile* self, TileSource* region, int x, int y, int z) {
    if(region->getData(x, y, z) == 0) {
	void (*super)(Tile*, TileSource*, int, int, int) = (void (*)(Tile*, TileSource*, int, int, int)) tiles[50]->vtable[VT_TILE_ONPLACE];
	super(tiles[50], region, x, y, z);
    }
    if(self->isActive()) {
	region->updateNeighborsAt(x, y - 1, z, notGate_on->id);
	region->updateNeighborsAt(x, y + 1, z, notGate_on->id);
	region->updateNeighborsAt(x - 1, y, z, notGate_on->id);
	region->updateNeighborsAt(x + 1, y, z, notGate_on->id);
	region->updateNeighborsAt(x, y, z - 1, notGate_on->id);
	region->updateNeighborsAt(x, y, z + 1, notGate_on->id);
    }
}

void NotGateTile::onRemove(NotGateTile* self, TileSource* region, int x, int y, int z) {
    if(self->isActive()) {
	region->scheduleBlockUpdate(x, y, z, self->id, 0);
    }
}

void NotGateTile::neighborChanged(NotGateTile* self, TileSource* region, int x, int y, int z, int xx, int yy, int zz) {
    if(!canSurvive(self, region, x, y, z)) {
	region->setTileAndData(x, y, z, 0, 0, 0);
	self->popResource(region, x, y, z, ItemInstance(tiles[self->getResource()], 1, 0));
    }
    region->scheduleBlockUpdate(x, y, z, self->id, 2);
}

void NotGateTile::tick(NotGateTile* self, TileSource* region, int x, int y, int z, Random* random) {
    bool burnt = self->checkForBurnout(region, x, y, z);
    if(burnt && region->getTile(x, y, z).id == notGate_on->id) {
	region->setTileAndData(x, y, z, 75, region->getData(x, y, z), 3);
	region->updateNeighborsAt(x, y + 1, z, 75);
    }
    else if(!burnt && region->getTile(x, y, z).id == 75)
	region->setTileAndData(x, y, z, notGate_on->id, region->getData(x, y, z), 3);
}

bool NotGateTile::isSignalSource(NotGateTile* tile) {
    return tile->isActive();
}

bool NotGateTile::checkForBurnout(TileSource* region, int x, int y, int z) {
    int data = region->getData(x, y, z);
    if((data == 5 || data == 0) && region->getIndirectPowerOutput(x, y - 1, z, 0)) return true;
    if(data == 3 && region->getIndirectPowerOutput(x, y, z - 1, 2)) return true;
    if(data == 4 && region->getIndirectPowerOutput(x, y, z + 1, 3)) return true;
    if(data == 1 && region->getIndirectPowerOutput(x - 1, y, z, 4)) return true;
    return data == 2 && region->getIndirectPowerOutput(x + 1, y, z, 5);
}

int NotGateTile::getResource() {
    return 76;
}

void NotGateTile::addCollisionShapes() {}