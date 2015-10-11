#include "TileItem.h"
#include "../level/tile/Tile.h"
#include "../../../addresses.h"

void** TileItem::_vtable;

TileItem::TileItem(int id, Tile* parent) : Item(id) {
    memcpy(vtable, items[20]->vtable, VT_ITEM_SIZE);
    parentId = parent->id;
}