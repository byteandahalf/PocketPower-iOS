#pragma once
#include "Item.h"
class Tile;

struct ItemInstance {
	short count, data;
	void* tag;
	Item* item;
	Tile* tile;
	int idk;

	ItemInstance(Item* item, short count, short data) : count(count), data(data), item(item) {}
	ItemInstance(int id, short count, short data) : count(count), data(data), item(Item::items[id]) {}
	ItemInstance(const ItemInstance& i) : count(i.count), data(i.data), tag(i.tag), item(i.item), tile(i.tile), idk(i.idk) {}
};