#include "TileItem.h"
#include "../level/tile/Tile.h"

TileItem::TileItem(int id, Tile* parent) : Item(id) {
    vtable = items[50]->vtable;
    parentId = parent->id;
}