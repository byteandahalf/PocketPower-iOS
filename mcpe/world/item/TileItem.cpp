#include "TileItem.h"
#include "../level/tile/Tile.h"
#include "../../../addresses.h"

TileItem::TileItem(int id, Tile* parent) : Item(id) {
    memcpy(vtable, items[50]->vtable, VT_ITEM_SIZE);
    parentId = parent->id;
}