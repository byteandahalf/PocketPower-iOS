#pragma once
#include "../../../mcpe/world/item/Item.h"
struct Player;

class RepeaterItem : public Item {
public:
    RepeaterItem(int);

private:
    void initVtable();

    static bool useOn(RepeaterItem*, ItemInstance*, Player*, int, int, int, signed char, float, float, float);
};