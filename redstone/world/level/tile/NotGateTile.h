#include "../../../../mcpe/world/level/tile/Tile.h"
class TileSource;
class Random;

class NotGateTile : public Tile {
public:
    NotGateTile(int, TextureUVCoordinateSet);

    bool isActive();
    bool checkForBurnout(TileSource*, int, int, int);

    static int getDirectSignal(NotGateTile*, TileSource*, int, int, int, int);
    static int getSignal(NotGateTile*, TileSource*, int, int, int, int);
    static bool isSignalSource(NotGateTile*);
    static void neighborChanged(NotGateTile*, TileSource*, int, int, int, int, int, int);
    static void addCollisionShapes();
    static void tick(NotGateTile*, TileSource*, int, int, int, Random*);
    static void onPlace(NotGateTile*, TileSource*, int, int, int);
    static void onRemove(NotGateTile*, TileSource*, int, int, int);
    static bool canSurvive(NotGateTile*, TileSource*, int, int, int);
    static int getResource();

    void initVtable();
};