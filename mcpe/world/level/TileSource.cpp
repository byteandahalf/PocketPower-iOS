#include <cmath>
#include "TileSource.h"
#include "TilePos.h"
#include "chunk/LevelChunk.h"
#include "TileTickingQueue.h"
#include "tile/Tile.h"
#include "../Facing.h"
#include "../../../addresses.h"

bool (*TileSource::$setTileAndData)(TileSource*, const TilePos&, FullTile, int);
FullTile (*TileSource::$getTile)(TileSource*, const TilePos&);
DataID (*TileSource::$getData)(TileSource*, const TilePos&);
void (*TileSource::$updateNeighborsAt)(TileSource*, const TilePos&, TileID);
EntityList& (*TileSource::$getEntities)(TileSource*, Entity*, const AABB&);

bool TileSource::setTileAndData(int x, int y, int z, int id, int data, int update) {
    return $setTileAndData(this, TilePos(x, y, z), FullTile(id, data), update);
}

FullTile TileSource::getTile(int x, int y, int z) {
    return $getTile(this, TilePos(x, y, z));
}

DataID TileSource::getData(int x, int y, int z) {
    return $getData(this, TilePos(x, y, z));
}

void TileSource::updateNeighborsAt(int x, int y, int z, int id) {
    $updateNeighborsAt(this, TilePos(x, y, z), id);
}

void TileSource::tileEvent(int x, int y, int z, int data1, int data2) {
    $tileEvent(this, x, y, z, data1, data2);
}

EntityList& TileSource::getEntities(Entity* entity, const AABB& pos) {
    return $getEntities(this, entity, pos);
}

void TileSource::scheduleBlockUpdate(int x, int y, int z, int id, int delay) {
    getTickingQueue()->add(this, TilePos(x, y, z), id, delay);
}

bool TileSource::isBlockIndirectlyGettingPowered(int x, int y, int z) {
    if(getIndirectPowerLevelTo(x, y - 1, z, 0) > 0) return true;
    if(getIndirectPowerLevelTo(x, y + 1, z, 1) > 0) return true;
    if(getIndirectPowerLevelTo(x, y, z - 1, 2) > 0) return true;
    if(getIndirectPowerLevelTo(x, y, z + 1, 3) > 0) return true;
    if(getIndirectPowerLevelTo(x - 1, y, z, 4) > 0) return true;
    return getIndirectPowerLevelTo(x + 1, y, z, 5);
}

int TileSource::getStrongestIndirectPower(int x, int y, int z) {
    int var4 = 0;

    for(int var5 = 0; var5 < 6; ++var5) {
        int var6 = getIndirectPowerLevelTo(x + Facing::STEP_X[var5], y + Facing::STEP_Y[var5], z + Facing::STEP_Z[var5], var5);
        if(var6 >= 15) return 15;
        if(var6 > var4) var4 = var6;
    }
    return var4;
}

int TileSource::getIndirectPowerLevelTo(int x, int y, int z, int side) {
    int id = getTile(x, y, z).id;
    if(Tile::solid[id] && id != 152) return getBlockPowerInput(x, y, z);
    if(id == 0) return 0;

    int (*getDirectSignal)(Tile*, TileSource*, int, int, int, int) = (int (*)(Tile*, TileSource*, int, int, int, int)) Tile::tiles[id]->vtable[VT_TILE_GETDIRECT];
    return getDirectSignal(Tile::tiles[id], this, x, y, z, side);
}

int TileSource::isBlockProvidingPowerTo(int x, int y, int z, int side) {
    int id = getTile(x, y, z).id;
    if(id == 0) return 0;

    int (*getSignal)(Tile*, TileSource*, int, int, int, int) = (int (*)(Tile*, TileSource*, int, int, int, int)) Tile::tiles[id]->vtable[VT_TILE_GETSIGNAL];
    return getSignal(Tile::tiles[id], this, x, y, z, side);
}

bool TileSource::getIndirectPowerOutput(int x, int y, int z, int side) {
    return getIndirectPowerLevelTo(x, y, z, side) > 0;
}

bool TileSource::isBlockGettingPowered(int x, int y, int z) {
    if(isBlockProvidingPowerTo(x, y - 1, z, 0)) return true;
    if(isBlockProvidingPowerTo(x, y + 1, z, 1)) return true;
    if(isBlockProvidingPowerTo(x, y, z - 1, 2)) return true;
    if(isBlockProvidingPowerTo(x, y, z + 1, 3)) return true;
    if(isBlockProvidingPowerTo(x - 1, y, z, 4)) return true;
    return isBlockProvidingPowerTo(x + 1, y, z, 5);
}

int max(int a, int b) {
    if(a > b) return a;
    return b;
}

int TileSource::getBlockPowerInput(int x, int y, int z) {
    int var5 = max(0, isBlockProvidingPowerTo(x, y - 1, z, 0));

    if(var5 >= 15) return var5;
    else {
        var5 = max(var5, isBlockProvidingPowerTo(x, y + 1, z, 1));
        if(var5 >= 15) return var5;
        else {
            var5 = max(var5, isBlockProvidingPowerTo(x, y, z - 1, 2));
            if(var5 >= 15) return var5;
            else {
                var5 = max(var5, isBlockProvidingPowerTo(x, y, z + 1, 3));
                if(var5 >= 15) return var5;
                else {
                    var5 = max(var5, isBlockProvidingPowerTo(x - 1, y, z, 4));
                    if(var5 >= 15) return var5;
                    else {
                        var5 = max(var5, isBlockProvidingPowerTo(x + 1, y, z, 5));
                        return var5;
                    }
                }
            }
        }
    }
}

TileTickingQueue* TileSource::getTickingQueue() {
    return &lastChunk->tickingQueue;
}