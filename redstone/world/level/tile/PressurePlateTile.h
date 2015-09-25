#pragma once

#include "../../../../mcpe/world/level/tile/Tile.h"
#include "../../../../mcpe/world/entity/Entity.h"
struct AABB;
class TileSource;

class PressurePlateTile : public Tile {
public:
    PressurePlateTile(int, TextureUVCoordinateSet, Material*);

    bool isWood();
    void setStateIfMobInteractsWithPlate(TileSource*, int, int, int);
    int getTickDelay();

    static bool _listContainsMob(EntityList&);
    static bool _isMob(Entity*);

    static void neighborChanged(PressurePlateTile*, TileSource*, int, int, int, int, int, int);
    static bool mayPlace(PressurePlateTile*, TileSource*, int, int, int);
    static void entityInside(PressurePlateTile*, TileSource*, int, int, int, Entity*);
    static void addCollisionShapes(PressurePlateTile*);
    static void onPlace(PressurePlateTile*, TileSource*, int, int, int);
    static void tick(PressurePlateTile*, TileSource*, int, int, int, Random*);
    static const AABB& getVisualShapeInWorld(PressurePlateTile*, TileSource*, int, int, int, AABB&, bool);
    static int getSignal(PressurePlateTile*, TileSource*, int, int, int, int);
    static int getDirectSignal(PressurePlateTile*, TileSource*, int, int, int, int);
    static bool isSignalSource(PressurePlateTile*);
    static void onRemove(PressurePlateTile*, TileSource*, int, int, int);

    void initVtable();
};