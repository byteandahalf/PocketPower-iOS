#pragma once
#include "../../../../mcpe/world/level/tile/Tile.h"

class RedstoneBlockTile : public Tile {
public:
    void initVtable();

    static int getSignal();
    static int getDirectSignal();
    static bool isSignalSource();
};