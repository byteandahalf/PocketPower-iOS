#pragma once
#include <Forklift/Forklift.h>
#include <stdint.h>
struct TilePos;
#include "../../CommonTypes.h"
#include "../entity/Entity.h"
class LevelChunk;
struct AABB;
struct TileTickingQueue;

#define TILESOURCE_FILLER FLArch(36, 36, 72)

class TileSource {
public:
    char filler[TILESOURCE_FILLER];
    LevelChunk* lastChunk;

    bool setTileAndData(int, int, int, int, int, int);
    FullTile getTile(int, int, int);
    DataID getData(int, int, int);
    void updateNeighborsAt(int, int, int, int);
    void scheduleBlockUpdate(int, int, int, int, int);

    bool isBlockIndirectlyGettingPowered(int, int, int);
    int isBlockProvidingPowerTo(int, int, int, int);
    int getIndirectPowerLevelTo(int, int, int, int);
    int getBlockPowerInput(int, int, int);
    bool getIndirectPowerOutput(int, int, int, int);
    bool isBlockGettingPowered(int, int, int);
    int getStrongestIndirectPower(int, int, int);
    TileTickingQueue* getTickingQueue();
    EntityList& getEntities(Entity*, const AABB&);
    void tileEvent(int, int, int, int, int);

    static bool (*$setTileAndData)(TileSource*, const TilePos&, FullTile, int);
    static FullTile (*$getTile)(TileSource*, const TilePos&);
    static DataID (*$getData)(TileSource*, const TilePos&);
    static void (*$updateNeighborsAt)(TileSource*, const TilePos&, TileID);
    static EntityList& (*$getEntities)(TileSource*, Entity*, const AABB&);
    static void (*$tileEvent)(TileSource*, int, int, int, int, int);
};