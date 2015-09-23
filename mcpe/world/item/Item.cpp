#include "Item.h"

Item** Item::items;
std::vector<ItemInstance>* Item::creativeList;

Item::Item(int id) {
    vtable = items[318]->vtable;
    maxStackSize = 64;
    //atlas = "items-opaque.png";
    frameCount = 1;
    animates = 1;
    this->id = id;
    maxDamage = 0;
    icon = TextureUVCoordinateSet(0.0, 0.0, 0.0312, 0.0312);
    maxDamage = 0;
    creativeTab = 2;
    renderAsSword = false;
    isStackedByData = false;
    iddk = 0;
    craftingRemainingItem = this;
    //name = "New item";

    items[id] = this;
}

void Item::addCreativeItem(Item* item, short data) {
    ItemInstance borrow = ItemInstance((*creativeList)[0]);
    borrow.item = item;
    borrow.data = data;
    creativeList->push_back(borrow);
}