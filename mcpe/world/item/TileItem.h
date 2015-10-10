#pragma once
#include "Item.h"
#include "../../CommonTypes.h"
class Tile;

class TileItem : public Item {
public:
	static void** _vtable;
	
    TileID parentId;

    TileItem(int, Tile*);
};