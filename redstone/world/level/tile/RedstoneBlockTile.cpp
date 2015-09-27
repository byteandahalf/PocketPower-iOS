#include "RedstoneBlockTile.h"
#include "../../../../addresses.h"

void RedstoneBlockTile::initVtable() {
    vtable = (void**) malloc(VT_TILE_SIZE);
    memcpy(vtable, tiles[4]->vtable, VT_TILE_SIZE);

    vtable[VT_TILE_GETSIGNAL] = (void*) &getSignal;
    vtable[VT_TILE_GETDIRECT] = (void*) &getDirectSignal;
    vtable[VT_TILE_ISSOURCE] = (void*) &isSignalSource;
}

int RedstoneBlockTile::getSignal() {
    return 0;
}

int RedstoneBlockTile::getDirectSignal() {
    return 15;
}

bool RedstoneBlockTile::isSignalSource() {
    return true;
}