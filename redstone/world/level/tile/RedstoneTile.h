#include "../../../../mcpe/world/level/tile/Tile.h"
class TileSource;
struct Player;

class RedstoneTile : public Tile {
public:
    bool wiresProvidePower;

    RedstoneTile(int);

    void calculateChanges(TileSource*, int, int, int, int, int, int);
    void recalculate(TileSource*, int, int, int);
    int getStrongerSignal(TileSource*, int, int, int, int);
    bool canRedstoneConnect(TileSource*, int, int, int, int);
    void updateWires(TileSource*, int, int, int);

    static int getColor(Tile*, TileSource*, int, int, int);
    static void addCollisionShapes();
    static void neighborChanged(RedstoneTile*, TileSource*, int, int, int, int, int, int);
    static int getDirectSignal(RedstoneTile*, TileSource*, int, int, int, int);
    static int getSignal(RedstoneTile*, TileSource*, int, int, int, int);
    static bool isSignalSource(RedstoneTile*);
    static void onPlace(RedstoneTile*, TileSource*, int, int, int);
    static void playerDestroy(RedstoneTile*, Player*, int, int, int);
    static bool canSurvive(RedstoneTile*, TileSource*, int, int, int);
    static bool mayPlace(RedstoneTile*, TileSource*, int, int, int);

    void initVtable();
};