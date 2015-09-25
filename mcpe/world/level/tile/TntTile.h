#pragma once
#include "Tile.h"
class TileSource;

class TntTile : public Tile {
public:
    static void initVtable(TntTile*);
    static void onPlace(TntTile*, TileSource*, int, int, int);
    static void neighborChanged(TntTile*, TileSource*, int, int, int, int, int, int);
};