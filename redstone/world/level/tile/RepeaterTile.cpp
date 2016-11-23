#include "RepeaterTile.h"
#include "../../../../mcpe/world/item/Item.h"
#include "../../../../mcpe/world/level/TileSource.h"

#include "../../../../mcpe/world/item/ItemInstance.h"
#include "../../../../mcpe/world/Facing.h"
#include "../../../../mcpe/world/entity/player/Player.h"
#include "../../../../addresses.h"
#include <cmath>

float RepeaterTile::torchOffset[4] = {-0.0625F, 0.0625F, 0.1875F, 0.3125F};
int RepeaterTile::delaySettings[4] = {1, 2, 3, 4};

RepeaterTile::RepeaterTile(int blockId, TextureUVCoordinateSet texture, bool powered) : Tile(blockId, texture, tiles[50]->material, "repeater") {
    init();

    side_texture = TextureUVCoordinateSet(0.625F, 0.0625F, 0.6562F, 0.125F);
    renderType = 15;
    renderPass = 7;
    solid[blockId] = false;
    lightBlock[blockId] = 0;
    hitbox.set(0.0F, 0.0F, 0.0F, 1.0F, 0.125F, 1.0F);
    this->powered = powered;

    initVtable();
}

void RepeaterTile::initVtable() {
    vtable[VT_TILE_TICK] = (void*) &tick;
    vtable[VT_TILE_TEXTURE] = (void*) &getTexture;
    vtable[VT_TILE_GETSIGNAL] = (void*) &getSignal;
    vtable[VT_TILE_GETDIRECT] = (void*) &getDirectSignal;
    vtable[VT_TILE_NEIGHBORCH] = (void*) &neighborChanged;
    vtable[VT_TILE_ISSOURCE] = (void*) &isSignalSource;
    vtable[VT_TILE_GETPLACEDATA] = (void*) &getPlacementDataValue;
    vtable[VT_TILE_ONPLACE] = (void*) &onPlace;
    vtable[VT_TILE_USE] = (void*) &use;
    vtable[VT_TILE_RESOURCE] = (void*) &getResource;
    vtable[VT_TILE_MAYPLACE] = (void*) &mayPlace;
}

bool RepeaterTile::mayPlace(RepeaterTile* self, TileSource* region, int x, int y, int z) {
    return solid[region->getTile(x, y - 1, z).id];
}

const TextureUVCoordinateSet& RepeaterTile::getTexture(RepeaterTile* self, signed char side, int data) {
    if(side == 1)
        return self->tex;
    return self->side_texture;
}

void RepeaterTile::tick(RepeaterTile* self, TileSource* region, int x, int y, int z, Random* random) {
    int data = region->getData(x, y, z);
    bool shouldBePowered = self->isGettingPowered(region, x, y, z, data);
    if(self->powered && !shouldBePowered)
        region->setTileAndData(x, y, z, 93, data, 3);
    else if(!self->powered) {
        region->setTileAndData(x, y, z, 94, data, 3);
        if(!shouldBePowered) {
            int delay = (data & 12) >> 2;
            region->scheduleBlockUpdate(x, y, z, 94, delaySettings[delay] * 2);
        }
    }
}

int RepeaterTile::getSignal(RepeaterTile* self, TileSource* region, int x, int y, int z, int side) {
    return getDirectSignal(self, region, x, y, z, side);
}

int RepeaterTile::getDirectSignal(RepeaterTile* self, TileSource* region, int x, int y, int z, int side) {
    if(!self->powered)
        return 0;

    int rot = region->getData(x, y, z) & 3;
    if(rot == 0 && side == 3)
        return 15;
    if(rot == 1 && side == 4)
        return 15;
    if(rot == 2 && side == 2)
        return 15;
    if(rot == 3 && side == 5)
        return 15;

    return 0;
}

void RepeaterTile::neighborChanged(RepeaterTile* self, TileSource* region, int x, int y, int z, int changedX, int changedY, int changedZ) {
    if(!mayPlace(self, region, x, y, z)) {
        region->setTileAndData(x, y, z, 0, 0, 3);
        //self->popResource(region, x, y, z, ItemInstance(Item::items[self->getResource()], 1, 0));
        return;
    }

    int data = region->getData(x, y, z);
    bool shouldBePowered = self->isGettingPowered(region, x, y, z, data);
    int delay = (data & 12) >> 2;
    if(self->powered && !shouldBePowered)
        region->scheduleBlockUpdate(x, y, z, self->id, delaySettings[delay] * 2);
    else if(!self->powered && shouldBePowered)
        region->scheduleBlockUpdate(x, y, z, self->id, delaySettings[delay] * 2);
}

bool RepeaterTile::isGettingPowered(TileSource* region, int x, int y, int z, int data) {
    int rot = data & 3;

    switch(rot) {
    case 0:
        if(region->getTile(x, y, z + 1).id == 55 && region->getData(x, y, z + 1) > 0)
            return true;
        return region->getIndirectPowerLevelTo(x, y, z + 1, 3) > 0;
    case 2:
        if(region->getTile(x, y, z - 1).id == 55 && region->getData(x, y, z - 1) > 0)
            return true;
        return region->getIndirectPowerLevelTo(x, y, z - 1, 2) > 0;
    case 3:
        if(region->getTile(x + 1, y, z).id == 55 && region->getData(x + 1, y, z) > 0)
            return true;
        return region->getIndirectPowerLevelTo(x + 1, y, z, 5) > 0;
    case 1:
        if(region->getTile(x - 1, y, z).id == 55 && region->getData(x - 1, y, z) > 0)
            return true;
        return region->getIndirectPowerLevelTo(x - 1, y, z, 4) > 0;
    }
    return false;
}

bool RepeaterTile::use(RepeaterTile* self, Player* player, int x, int y, int z) {
    int data = player->region.getData(x, y, z);
    int delay = (data & 12) >> 2;
    delay = ((delay + 1) << 2) & 12;
    player->region.setTileAndData(x, y, z, self->id, delay | (data & 3), 3);
    return true;
}

bool RepeaterTile::isSignalSource() {
    return true;
}

int RepeaterTile::getPlacementDataValue(RepeaterTile* self, Mob* placer, int x, int y, int z, signed char side) {
    return (((int) floor((placer->yaw * 4.0F / 360.0F) + 0.5F) & 3) + 2) % 4;
}

void RepeaterTile::onPlace(RepeaterTile* self, TileSource* region, int x, int y, int z) {
    if(self->isGettingPowered(region, x, y, z, region->getData(x, y, z)))
        region->scheduleBlockUpdate(x, y, z, self->id, 1);

    region->updateNeighborsAt(x + 1, y, z, self->id);
    region->updateNeighborsAt(x - 1, y, z, self->id);
    region->updateNeighborsAt(x, y, z + 1, self->id);
    region->updateNeighborsAt(x, y, z - 1, self->id);
    region->updateNeighborsAt(x, y - 1, z, self->id);
    region->updateNeighborsAt(x, y + 1, z, self->id);
}

int RepeaterTile::getResource() {
    return 93;
}
