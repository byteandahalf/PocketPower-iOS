#include "TileTickingQueue.h"

void (*TileTickingQueue::$add)(TileTickingQueue*, TileSource*, const TilePos&, TileID, int);

void TileTickingQueue::add(TileSource* region, const TilePos& pos, TileID id, int delay) {
	$add(this, region, pos, id, delay);
}