#include "RedstoneTile.h"
#include "../../../../mcpe/world/level/TileSource.h"
#include "../../../../mcpe/world/level/TilePos.h"

#include "../../../../mcpe/CommonTypes.h"
#include "../../../../addresses.h"

#include "RedstoneColors.h"

RedstoneTile::RedstoneTile(int id) : Tile(id, TextureUVCoordinateSet(0.125, 0.5, 0.1562, 0.5625), tiles[50]->material) {
    init();

    solid[id] = false;
    lightBlock[id] = 0;
    hitbox.f5 = 0.0001F;
    renderType = 5;
    renderPass = 3;
    destroyTime = 0.0F;

    initVtable();
}

void RedstoneTile::initVtable() {
    vtable[VT_TILE_GETCOLOR] = (void*) &getColor;
    vtable[VT_TILE_ADDCOLLISION] = (void*) &addCollisionShapes;
    vtable[VT_TILE_NEIGHBORCH] = (void*) &neighborChanged;
    vtable[VT_TILE_ISSOURCE] = (void*) &isSignalSource;
    vtable[VT_TILE_ONPLACE] = (void*) &onPlace;
    vtable[VT_TILE_REMOVE] = (void*) &onRemove;
    vtable[VT_TILE_GETDIRECT] = (void*) &getDirectSignal;
    vtable[VT_TILE_GETSIGNAL] = (void*) &getSignal;
    vtable[VT_TILE_SURVIVES] = (void*) &canSurvive;
    vtable[VT_TILE_MAYPLACE] = (void*) &mayPlace;
}

bool RedstoneTile::canSurvive(RedstoneTile* self, TileSource* region, int x, int y, int z) {
    return solid[region->getTile(x, y - 1, z).id];
}

bool RedstoneTile::mayPlace(RedstoneTile* self, TileSource* region, int x, int y, int z) {
    if(region->getTile(x, y, z).id != 0) return false;
    // TODO: Implement placement exceptions for slabs, glowstone
    return solid[region->getTile(x, y - 1, z).id];
}

int RedstoneTile::getColor(Tile* self, TileSource* region, int x, int y, int z) {
    return colors[region->getData(x, y, z)];
}

void RedstoneTile::onPlace(RedstoneTile* tile, TileSource* region, int x, int y, int z) {
    tile->recalculate(region, x, y, z);
    region->updateNeighborsAt(x, y + 1, z, tile->id);
    region->updateNeighborsAt(x, y - 1, z, tile->id);
    tile->updateWires(region, x - 1, y, z);
    tile->updateWires(region, x + 1, y, z);
    tile->updateWires(region, x, y, z - 1);
    tile->updateWires(region, x, y, z + 1);
    if(solid[region->getTile(x - 1, y, z).id])
        tile->updateWires(region, x - 1, y + 1, z);
    else
        tile->updateWires(region, x - 1, y - 1, z);
    if(solid[region->getTile(x + 1, y, z).id])
        tile->updateWires(region, x + 1, y + 1, z);
    else
        tile->updateWires(region, x + 1, y - 1, z);
    if(solid[region->getTile(x, y, z - 1).id])
        tile->updateWires(region, x, y + 1, z - 1);
    else
        tile->updateWires(region, x, y - 1, z - 1);
    if(solid[region->getTile(x, y, z + 1).id])
        tile->updateWires(region, x, y + 1, z + 1);
    else
        tile->updateWires(region, x, y - 1, z + 1);
}

void RedstoneTile::onRemove(RedstoneTile* tile, TileSource* region, int x, int y, int z) {
    region->updateNeighborsAt(x, y + 1, z, tile->id);
    region->updateNeighborsAt(x, y - 1, z, tile->id);
    tile->recalculate(region, x, y, z);
    tile->updateWires(region, x - 1, y, z);
    tile->updateWires(region, x + 1, y, z);
    tile->updateWires(region, x, y, z - 1);
    tile->updateWires(region, x, y, z + 1);
    if(solid[region->getTile(x - 1, y, z).id])
        tile->updateWires(region, x - 1, y + 1, z);
    else
        tile->updateWires(region, x - 1, y - 1, z);
    if(solid[region->getTile(x + 1, y, z).id])
        tile->updateWires(region, x + 1, y + 1, z);
    else
        tile->updateWires(region, x + 1, y - 1, z);
    if(solid[region->getTile(x, y, z - 1).id])
        tile->updateWires(region, x, y + 1, z - 1);
    else
        tile->updateWires(region, x, y - 1, z - 1);
    if(solid[region->getTile(x, y, z + 1).id])
        tile->updateWires(region, x, y + 1, z + 1);
    else
        tile->updateWires(region, x, y - 1, z + 1);
}

void RedstoneTile::calculateChanges(TileSource* region, int x, int y, int z, int xx, int yy, int zz) {
    int oldPower = region->getData(x, y, z);
    int newPower = getStrongerSignal(region, xx, yy, zz, 0);
    wiresProvidePower = false;
    int receivedPower = region->getStrongestIndirectPower(x, y, z);
    wiresProvidePower = true;
    
    if(receivedPower > 0 && receivedPower > newPower - 1)
        newPower = receivedPower;
        
    int temp = 0;
        
    for(int it = 0; it < 4; ++it) {
        int newX = x;
        int newZ = z;
        
        if(it == 0)
            newX = x - 1;
        if(it == 1)
            ++newX;
        if(it == 2)
            newZ = z - 1;
        if(it == 3)
            ++newZ;
            
        if(newX != xx || newZ != zz)
            temp = getStrongerSignal(region, newX, y, newZ, temp);
            
        if(solid[region->getTile(newX, y, newZ).id] && !solid[region->getTile(x, y + 1, z).id]) {
            if((newX != xx || newZ != zz) && y >= yy)
                temp = getStrongerSignal(region, newX, y + 1, newZ, temp);
        }
        else if(!solid[region->getTile(newX, y, newZ).id] && (newX != xx || newZ != zz) && y <= yy)
            temp = getStrongerSignal(region, newX, y - 1, newZ, temp);
    }
    
    if(temp > newPower)
        newPower = temp - 1;
    else if(newPower > 0)
        --newPower;
    else
        newPower = 0;
        
    if(receivedPower > newPower - 1)
        newPower = receivedPower;
        
    if(oldPower != newPower) {
        region->setTileAndData(x, y, z, id, newPower, 2);
        region->updateNeighborsAt(x, y, z, id);
        region->updateNeighborsAt(x - 1, y, z, id);
        region->updateNeighborsAt(x + 1, y, z, id);
        region->updateNeighborsAt(x, y - 1, z, id);
        region->updateNeighborsAt(x, y + 1, z, id);
        region->updateNeighborsAt(x, y, z - 1, id);
        region->updateNeighborsAt(x, y, z + 1, id);
    }
}

void RedstoneTile::recalculate(TileSource* region, int x, int y, int z) {
    calculateChanges(region, x, y, z, x, y, z);
}


void RedstoneTile::neighborChanged(RedstoneTile* self, TileSource* region, int x, int y, int z, int xx, int yy, int zz) {
    self->recalculate(region, x, y, z);

    void (*super)(Tile*, TileSource*, int, int, int, int, int, int) = (void (*)(Tile*, TileSource*, int, int, int, int, int, int)) Tile::tiles[1]->vtable[VT_TILE_NEIGHBORCH];
    super(self, region, x, y, z, xx, yy, zz);
}

int RedstoneTile::getStrongerSignal(TileSource* region, int x, int y, int z, int signal) {
    if(region->getTile(x, y, z).id != id) return signal;
    int signal2 = region->getData(x, y, z);
    if(signal2 > signal) return signal2;
    return signal;
}

int RedstoneTile::getDirectSignal(RedstoneTile* tile, TileSource* region, int x, int y, int z, int side) {
    if(!tile->wiresProvidePower) return 0;
    int data = region->getData(x, y, z);
    if(data == 0) return 0;
    if(side == 1) return data;
    bool xm = tile->canRedstoneConnect(region, x - 1, y, z, 1) || (!Tile::solid[region->getTile(x - 1, y, z).id] && tile->canRedstoneConnect(region, x - 1, y - 1, z, -1));
    bool xp = tile->canRedstoneConnect(region, x + 1, y, z, 3) || (!Tile::solid[region->getTile(x + 1, y, z).id] && tile->canRedstoneConnect(region, x + 1, y - 1, z, -1));
    bool zm = tile->canRedstoneConnect(region, x, y, z - 1, 2) || (!Tile::solid[region->getTile(x, y, z - 1).id] && tile->canRedstoneConnect(region, x, y - 1, z - 1, -1));
    bool zp = tile->canRedstoneConnect(region, x, y, z + 1, 0) || (!Tile::solid[region->getTile(x, y, z + 1).id] && tile->canRedstoneConnect(region, x, y - 1, z + 1, -1));

    if(!Tile::solid[region->getTile(x, y + 1, z).id]) {
        if(Tile::solid[region->getTile(x - 1, y, z).id] && tile->canRedstoneConnect(region, x - 1, y + 1, z, -1)) xm = true;
        if(Tile::solid[region->getTile(x + 1, y, z).id] && tile->canRedstoneConnect(region, x + 1, y + 1, z, -1)) xp = true;
        if(Tile::solid[region->getTile(x, y, z - 1).id] && tile->canRedstoneConnect(region, x, y + 1, z - 1, -1)) zm = true;
        if(Tile::solid[region->getTile(x, y, z + 1).id] && tile->canRedstoneConnect(region, x, y + 1, z + 1, -1)) zp = true;
    }
    if(!zm && !xp && !xm && !zp && side >= 2 && side <= 5) return data;
    if(side == 2 && zm && !xm && !xp) return data;
    if(side == 3 && zp && !xm && !xp) return data;
    if(side == 4 && xm && !zm && !zp) return data;
    if(side == 5 && xp && !zm && !zp) return data;
    return 0;
}

int RedstoneTile::getSignal(RedstoneTile* tile, TileSource* region, int x, int y, int z, int side) {
    if(!tile->wiresProvidePower) return 0;
    return getDirectSignal(tile, region, x, y, z, side);
}

bool RedstoneTile::canRedstoneConnect(TileSource* region, int x, int y, int z, int side) {
    int id = region->getTile(x, y, z).id;
    if(id == this->id) return true;
    if(id == 0) return false;
    bool (*isSignalSource)(Tile*) = (bool (*)(Tile*)) tiles[id]->vtable[VT_TILE_ISSOURCE];
    return isSignalSource(tiles[id]) && side != -1;
}

bool RedstoneTile::isSignalSource(RedstoneTile* tile) {
    return tile->wiresProvidePower;
}

void RedstoneTile::updateWires(TileSource* region, int x, int y, int z) {
    if(region->getTile(x, y, z).id != id) return;
    region->updateNeighborsAt(x, y, z, id);
    region->updateNeighborsAt(x - 1, y, z, id);
    region->updateNeighborsAt(x + 1, y, z, id);
    region->updateNeighborsAt(x, y - 1, z, id);
    region->updateNeighborsAt(x, y + 1, z, id);
    region->updateNeighborsAt(x, y, z - 1, id);
    region->updateNeighborsAt(x, y, z + 1, id);
}

void RedstoneTile::addCollisionShapes() {}