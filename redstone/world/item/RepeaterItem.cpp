#include "RepeaterItem.h"
#include "../../../mcpe/client/renderer/texture/TextureUVCoordinateSet.h"
#include "../../../mcpe/world/entity/player/Player.h"
#include "../../../mcpe/world/level/TileSource.h"
#include "../../../mcpe/world/Facing.h"
#include "../level/tile/RepeaterTile.h"
#include "../../../addresses.h"

RepeaterItem::RepeaterItem(int itemId) : Item(itemId) {
    creativeTab = 4;
    icon = TextureUVCoordinateSet(0.0625F, 0.2188F, 0.0937F, 0.25F);

    initVtable();
}

void RepeaterItem::initVtable() {
    vtable[VT_ITEM_USEON] = (void*) &useOn;
}

bool RepeaterItem::useOn(RepeaterItem* self, ItemInstance* item, Player* player, int x, int y, int z, signed char side, float xx, float yy, float zz) {
    int data = RepeaterTile::getPlacementDataValue((RepeaterTile*) Tile::diode_off, player, x, y, z, side);

    player->region.setTileAndData(x + Facing::STEP_X[side], y + Facing::STEP_Y[side], z + Facing::STEP_Z[side], Tile::diode_off->id, data, 2);
    return true;
}