#include "TileTessellator.h"

bool (*TileTessellator::$tessellateBlockInWorld)(TileTessellator*, Tile*, const TilePos&);

bool TileTessellator::tessellateBlockInWorld(Tile* tile, const TilePos& pos) {
	return $tessellateBlockInWorld(this, tile, pos);
}