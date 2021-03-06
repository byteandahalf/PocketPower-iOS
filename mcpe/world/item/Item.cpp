#include "Item.h"

void** Item::_vtable;
Item** Item::items;
std::vector<ItemInstance>* Item::creativeList;

Item* Item::repeater;

Item::Item(int id, const std::string& name) {
    vtable = (void**) malloc(VT_ITEM_SIZE);
    memcpy(vtable, Item::_vtable, VT_ITEM_SIZE);
    maxStackSize = 64;
    atlas = "items-opaque.png";
    frameCount = 1;
    animates = 1;
    this->id = id;
    maxDamage = 0;
    icon = TextureUVCoordinateSet(0.0, 0.0, 0.0312, 0.0312);
    maxDamage = 0;
    creativeTab = 2;
    renderAsSword = false;
    isStackedByData = false;
    craftingRemainingItem = this;
    this->name = name;

    items[id] = this;
}

void Item::addCreativeItem(Item* item, short data) {
    ItemInstance borrow = ItemInstance((*creativeList)[0]);
    borrow.item = item;
    borrow.data = data;
    creativeList->push_back(borrow);
}