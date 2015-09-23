#include "NotGateTile.h"
#include "../../../../addresses.h"
#include "../../../../mcpe/world/level/TileSource.h"

NotGateTile::NotGateTile(int id, TextureUVCoordinateSet texture) : Tile(id, texture, tiles[50]->material) {
    init();

    memcpy(vtable, tiles[50]->vtable, VT_TILE_SIZE);

    if(isActive())
        renderPass = 7;

    renderType = 2;
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

int NotGateTile::getSignal(NotGateTile* tile, TileSource* region, int x, int y, int z, int side) {
    if(side == 0) return getDirectSignal(tile, region, x, y, z, side);
    return 0;
}

void NotGateTile::onPlace(NotGateTile* self, TileSource* region, int x, int y, int z) {
    if(region->getData(x, y, z) == 0) //TorchTile::onPlace(region, x, y, z);
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
    if(data == 5 && region->getIndirectPowerOutput(x, y - 1, z, 0)) return true;
    if(data == 3 && region->getIndirectPowerOutput(x, y, z - 1, 2)) return true;
    if(data == 4 && region->getIndirectPowerOutput(x, y, z + 1, 3)) return true;
    if(data == 1 && region->getIndirectPowerOutput(x - 1, y, z, 4)) return true;
    return data == 2 && region->getIndirectPowerOutput(x + 1, y, z, 5);
}

void NotGateTile::addCollisionShapes() {}