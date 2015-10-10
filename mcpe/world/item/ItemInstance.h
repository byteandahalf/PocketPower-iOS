#pragma once
class Tile;
class Item;

struct ItemInstance {
	short count, data;
	void* tag;
	Item* item;
	Tile* tile;
	int idk;

	ItemInstance(Item* item, short count, short data) : count(count), data(data), item(item) {}
	ItemInstance(Tile* tile, short count, short data) : count(count), data(data), tile(tile) {}
	ItemInstance(const ItemInstance& i) : count(i.count), data(i.data), tag(i.tag), item(i.item), tile(i.tile), idk(i.idk) {}
};