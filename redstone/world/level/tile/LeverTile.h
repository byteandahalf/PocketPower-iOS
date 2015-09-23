#pragma once

#include "../../../../mcpe/world/level/tile/Tile.h"
struct AABB;
class Random;
struct Player;
class TileSource;
class Mob;


class LeverTile : public Tile {
public:
    LeverTile(int);

    static bool isSignalSource();
    static int getPlacementDataValue(LeverTile*, Mob*, int, int, int, signed char, float, float, float, int);
    static bool use(LeverTile*, Player*, int, int, int);
    static void neighborChanged(LeverTile*, TileSource*, int, int, int, int, int, int);
    static void tick(LeverTile*, TileSource*, int, int, int, Random*);
    static void onPlace(LeverTile*, TileSource*, int, int, int);
    static bool mayPlace(LeverTile*, TileSource*, int, int, int, signed char);
    static bool canSurvive(LeverTile*, TileSource*, int, int, int);
    static int getSignal(LeverTile*, TileSource*, int, int, int, int);
    static int getDirectSignal(LeverTile*, TileSource*, int, int, int, int);
    static const AABB& getVisualShape(LeverTile*, TileSource*, int, int, int, AABB&, bool);
    static void addCollisionShapes();

    void initVtable();
};