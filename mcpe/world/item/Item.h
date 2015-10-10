#pragma once
#include <string>
#include <vector>
#include "../../client/renderer/texture/TextureUVCoordinateSet.h"
#include "ItemInstance.h"
#include "../../../addresses.h"
class Tile;

class Item {
public:
    static void** _vtable;
    static Item** items;
    static std::vector<ItemInstance>* creativeList;

    static Item* repeater;

    Item(int);

    static void addCreativeItem(Item*, short);

    void** vtable; // 0
    long maxStackSize; // 8
    char atlas[SIZEOF_STRING]; // 16
    int frameCount; // 40
    int animates; // 44
    char name[SIZEOF_STRING]; // 48
    short id; // 72
    short maxDamage; // 74
    TextureUVCoordinateSet icon; // 76
    int creativeTab; // 108
    bool renderAsSword; // 112
    bool isStackedByData; // 113
    Item* craftingRemainingItem; // 120
};