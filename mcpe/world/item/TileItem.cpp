#include "TileItem.h"
#include "../level/tile/Tile.h"
#include "../../../addresses.h"

void** TileItem::_vtable;

TileItem::TileItem(int id, Tile* parent) : Item(id) {
    memcpy(vtable, TileItem::_vtable, VT_ITEM_SIZE);
    parentId = parent->id;
}