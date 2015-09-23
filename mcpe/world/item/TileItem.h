#pragma once
#include "Item.h"
#include "../../CommonTypes.h"
class Tile;

class TileItem : public Item {
public:
    TileID parentId;

    TileItem(int, Tile*);
};