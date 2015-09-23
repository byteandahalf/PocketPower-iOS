#include <substrate.h>
#include <Forklift/Forklift.h>
#include <string>
#include "addresses.h"

#include "mcpe/client/renderer/tile/TileTessellator.h"
#include "mcpe/client/renderer/renderer/Tessellator.h"
#include "mcpe/world/level/tile/Tile.h"
#include "mcpe/world/level/TileSource.h"
#include "mcpe/world/level/TileTickingQueue.h"
#include "mcpe/world/level/TilePos.h"
#include "mcpe/world/entity/player/Player.h"
#include "mcpe/world/item/Item.h"
#include "mcpe/world/item/ItemInstance.h"
#include "mcpe/CommonTypes.h"
#include "mcpe/world/Facing.h"

#include "redstone/world/level/tile/RedstoneTile.h"
#include "redstone/world/level/tile/NotGateTile.h"
#include "redstone/world/level/tile/ButtonTile.h"
#include "redstone/world/level/tile/PressurePlateTile.h"

using namespace std;


MSHook(bool, TileTessellator$tessellateInWorld, TileTessellator* self, Tile* tile, const TilePos& pos, int data, bool b) {
	switch(tile->renderType) {
	case 5:
		return self->tessellateRedstoneInWorld((RedstoneTile*) tile, pos, data);
	}

	switch(tile->id) {
	case 76:
		return self->tessellateLitNotGateInWorld((NotGateTile*) tile, pos, data);
	}

	return _TileTessellator$tessellateInWorld(self, tile, pos, data, b);
}

MSHook(void, Tile$initTiles) {
	_Tile$initTiles();

	Tile::redstoneDust = new RedstoneTile(55);
	Tile::notGate_on = new NotGateTile(76, TextureUVCoordinateSet(0.5938, 0.3125, 0.625, 0.375));
	Tile::notGate_off = new NotGateTile(75, TextureUVCoordinateSet(0.625, 0.3125, 0.6562, 0.375));
	Tile::buttonStone = new ButtonTile(77, TextureUVCoordinateSet(0.5938, 0.0, 0.625, 0.0625), Tile::tiles[50]->material);
	Tile::plateStone = new PressurePlateTile(70, TextureUVCoordinateSet(0.5938, 0.0, 0.625, 0.0625), Tile::tiles[50]->material);
	Tile::plateWood = new PressurePlateTile(72, TextureUVCoordinateSet(0.4375, 0.0625, 0.4687, 0.125), Tile::tiles[50]->material);
}

MSHook(bool, Item$useOn, Item* self, ItemInstance* item, Player* player, int x, int y, int z, signed char side, float xx, float yy, float zz) {
	if(item->item == Item::items[331]) {
	    if(((RedstoneTile*) Tile::tiles[55])->mayPlace((RedstoneTile*) Tile::tiles[55], &player->region, x + Facing::STEP_X[side], y + Facing::STEP_Y[side], z + Facing::STEP_Z[side]))
			player->region.setTileAndData(x + Facing::STEP_X[side], y + Facing::STEP_Y[side], z + Facing::STEP_Z[side], 55, 0, 3);
	}
	return _Item$useOn(self, item, player, x, y, z, side, xx, yy, zz);
}

MSHook(void, Item$initCreativeItems) {
	for(int i = 256; i < 512; i++) {
		if(!Item::items[i]) continue;
		if(Item::items[i]->creativeTab == 4)
		    Item::items[i]->creativeTab = 3;
	}
	Tile::tiles[66]->creativeTab = 4;
	Tile::tiles[27]->creativeTab = 4;
	Tile::tiles[152]->creativeTab = 4;
	Tile::tiles[46]->creativeTab = 4;
	Item::items[331]->creativeTab = 4;

	_Item$initCreativeItems();

	Item::addCreativeItem(Item::items[76], 0);
	Item::addCreativeItem(Item::items[70], 0);
	Item::addCreativeItem(Item::items[72], 0);
	Item::addCreativeItem(Item::items[77], 0);
};

MSInitialize {
	FLHookSymbol(Tile::_vtable, TILE_VTABLE);
	FLHookSymbol(Tile::$init, TILE_INIT);
	FLHookSymbol(Tile::tiles, TILE_TILES);
	FLHookSymbol(Tile::solid, TILE_SOLID);
	FLHookSymbol(Tile::lightBlock, TILE_LIGHTBLOCK);
	FLHookSymbol(Tile::shouldTick, TILE_SHOULDTICK);
	FLHookSymbol(Item::items, ITEM_ITEMS);
	FLHookSymbol(TileSource::$setTileAndData, TILESOURCE_SETTILE);
	FLHookSymbol(TileSource::$getTile, TILESOURCE_GETTILE);
	FLHookSymbol(TileSource::$getData, TILESOURCE_GETDATA);
	FLHookSymbol(TileSource::$updateNeighborsAt, TILESOURCE_UPDATE);
	FLHookSymbol(TileSource::$getEntities, TILESOURCE_GETENTITIES);
	FLHookSymbol(TileTessellator::$tessellateBlockInWorld, TILETESS_TESSBLOCK);
	FLHookSymbol(Tessellator::$vertex, TESS_VERTEX);
	FLHookSymbol(TileTickingQueue::$add, TICKQUEUE_ADD);
	FLHookSymbol(Item::creativeList, ITEM_CREATIVELIST);

	FLHookFunction(TILETESS_TESS, MSHake(TileTessellator$tessellateInWorld));
	FLHookFunction(TILE_INITTILES, MSHake(Tile$initTiles));
	FLHookFunction(ITEM_USEON, MSHake(Item$useOn));
	FLHookFunction(ITEM_INITCREATIVE, MSHake(Item$initCreativeItems));
}