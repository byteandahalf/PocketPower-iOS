#include <substrate.h>
#include <Forklift/Forklift.h>
#include <string>
#include "addresses.h"

#include "mcpe/client/renderer/tile/TileTessellator.h"
#include "mcpe/client/renderer/renderer/Tessellator.h"
#include "mcpe/world/level/tile/Tile.h"
#include "mcpe/world/level/tile/TntTile.h"
#include "mcpe/world/level/TileSource.h"
#include "mcpe/world/level/TileTickingQueue.h"
#include "mcpe/world/level/TilePos.h"
#include "mcpe/world/entity/player/Player.h"
#include "mcpe/world/item/Item.h"
#include "mcpe/world/item/TileItem.h"
#include "mcpe/world/item/ItemInstance.h"
#include "mcpe/CommonTypes.h"
#include "mcpe/world/Facing.h"

#include "redstone/world/item/RepeaterItem.h"

#include "redstone/world/level/tile/RedstoneTile.h"
#include "redstone/world/level/tile/NotGateTile.h"
#include "redstone/world/level/tile/ButtonTile.h"
#include "redstone/world/level/tile/LightPressurePlateTile.h"
#include "redstone/world/level/tile/HeavyPressurePlateTile.h"
#include "redstone/world/level/tile/LeverTile.h"
#include "redstone/world/level/tile/LampTile.h"
#include "redstone/world/level/tile/RedstoneBlockTile.h"
#include "redstone/world/level/tile/RepeaterTile.h"
#include "redstone/world/level/tile/PistonBaseTile.h"
#include "redstone/world/level/tile/PistonArmTile.h"
#include "redstone/world/level/tile/DoorTile.h"

using namespace std;

MSHook(bool, TileTessellator$tessellateInWorld, TileTessellator* self, Tile* tile, const TilePos& pos, int data, bool b) {
	switch(tile->renderType) {
	case 5:
		return self->tessellateRedstoneInWorld((RedstoneTile*) tile, pos, data);
	case 12:
		return self->tessellateLeverInWorld((LeverTile*) tile, pos);
	case 15:
		return self->tessellateRepeaterInWorld((RepeaterTile*) tile, pos, data);
	case 17:
		return self->tessellatePistonArmInWorld((PistonArmTile*) tile, pos, true);
	case 100:
		return self->tessellateLitNotGateInWorld((NotGateTile*) tile, pos, data);
	}

	switch(tile->id) {
	case 29:
	case 33:
		return self->tessellatePistonBaseInWorld((PistonBaseTile*) tile, pos, false);
	}

	return _TileTessellator$tessellateInWorld(self, tile, pos, data, b);
}

void initTileItems() {
	new TileItem(55, Tile::redstoneDust);
	new TileItem(69, Tile::lever);
	new TileItem(70, Tile::plateStone);
	new TileItem(72, Tile::plateWood);
	new TileItem(147, Tile::plateGold);
	new TileItem(148, Tile::plateIron);
	new TileItem(75, Tile::notGate_off);
	new TileItem(76, Tile::notGate_on);
	new TileItem(77, Tile::buttonStone);
	new TileItem(143, Tile::buttonWood);
	new TileItem(123, Tile::lamp_off);
	new TileItem(124, Tile::lamp_on);
	new TileItem(93, Tile::diode_off);
	new TileItem(94, Tile::diode_on);
	new TileItem(33, Tile::pistonNormal);
	new TileItem(29, Tile::pistonSticky);
	new TileItem(34, Tile::pistonArm);
}

MSHook(void, Tile$initTiles) {
	_Tile$initTiles();

	TntTile::initVtable((TntTile*) Tile::tiles[46]);
	((RedstoneBlockTile*) Tile::tiles[152])->initVtable();
	((DoorTile*) Tile::tiles[64])->initVtable();

	Tile::redstoneDust = new RedstoneTile(55);
	Tile::lever = new LeverTile(69);
	Tile::plateStone = new LightPressurePlateTile(70, TextureUVCoordinateSet(0.5938, 0.0, 0.625, 0.0625), Tile::tiles[50]->material, LightPressurePlateTile::SENSITIVITY::MOBS);
	Tile::plateWood = new LightPressurePlateTile(72, TextureUVCoordinateSet(0.4375, 0.0625, 0.4687, 0.125), Tile::tiles[50]->material, LightPressurePlateTile::SENSITIVITY::EVERYTHING);
	Tile::plateGold = new HeavyPressurePlateTile(147, TextureUVCoordinateSet(0.2813F, 0.1875F, 0.3125F, 0.25F), Tile::tiles[50]->material, 15);
	Tile::plateIron = new HeavyPressurePlateTile(148, TextureUVCoordinateSet(0.25F, 0.1875F, 0.2812F, 0.25F), Tile::tiles[50]->material, 150);
	Tile::notGate_off = new NotGateTile(75, TextureUVCoordinateSet(0.625, 0.3125, 0.6562, 0.375));
	Tile::notGate_on = new NotGateTile(76, TextureUVCoordinateSet(0.5938, 0.3125, 0.625, 0.375));
	Tile::buttonStone = new ButtonTile(77, TextureUVCoordinateSet(0.5938, 0.0, 0.625, 0.0625), Tile::tiles[50]->material);
	Tile::buttonWood = new ButtonTile(143, TextureUVCoordinateSet(0.4375, 0.0625, 0.4687, 0.125), Tile::tiles[50]->material);
	Tile::lamp_off = new LampTile(123, TextureUVCoordinateSet(0.25, 0.5, 0.2812, 0.5625));
	Tile::lamp_on = new LampTile(124, TextureUVCoordinateSet(0.2813, 0.5, 0.3125, 0.5625));
	Tile::diode_off = new RepeaterTile(93, TextureUVCoordinateSet(0.9063, 0.4375, 0.9375, 0.5), false);
	Tile::diode_on = new RepeaterTile(94, TextureUVCoordinateSet(0.9375, 0.4375, 0.9687, 0.5), true);
	Tile::pistonNormal = new PistonBaseTile(33, false);
	Tile::pistonSticky = new PistonBaseTile(29, true);
	Tile::pistonArm = new PistonArmTile(34);

	initTileItems();
}

MSHook(void, Item$initItems) {
    _Item$initItems();

    Item::repeater = new RepeaterItem(356);
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
		if(Item::items[i]->creativeTab == 4 && i != 356)
		    Item::items[i]->creativeTab = 3;
	}
	Tile::tiles[66]->creativeTab = 4;
	Tile::tiles[27]->creativeTab = 4;
	Tile::tiles[152]->creativeTab = 4;
	Tile::tiles[46]->creativeTab = 4;
	Item::items[324]->creativeTab = 4;
	Item::items[331]->creativeTab = 4;

	_Item$initCreativeItems();

	Item::addCreativeItem(Item::items[356], 0);
	Item::addCreativeItem(Item::items[76], 0);
	Item::addCreativeItem(Item::items[70], 0);
	Item::addCreativeItem(Item::items[72], 0);
	Item::addCreativeItem(Item::items[147], 0);
	Item::addCreativeItem(Item::items[148], 0);
	Item::addCreativeItem(Item::items[77], 0);
	Item::addCreativeItem(Item::items[143], 0);
	Item::addCreativeItem(Item::items[69], 0);
	Item::addCreativeItem(Item::items[123], 0);
	Item::addCreativeItem(Item::items[33], 0);
	Item::addCreativeItem(Item::items[29], 0);
	Item::addCreativeItem(Item::items[330], 0);
};


MSInitialize {
	FLHookSymbol(Tile::_vtable, TILE_VTABLE);
	FLHookSymbol(Tile::$init, TILE_INIT);
	FLHookSymbol(Tile::tiles, TILE_TILES);
	FLHookSymbol(Tile::solid, TILE_SOLID);
	FLHookSymbol(Tile::lightBlock, TILE_LIGHTBLOCK);
	FLHookSymbol(Tile::shouldTick, TILE_SHOULDTICK);
	FLHookSymbol(Item::_vtable, ITEM_VTABLE);
	FLHookSymbol(Item::items, ITEM_ITEMS);
	FLHookSymbol(TileItem::_vtable, TILEITEM_VTABLE);
	FLHookSymbol(TileSource::$setTileAndData, TILESOURCE_SETTILE);
	FLHookSymbol(TileSource::$getTile, TILESOURCE_GETTILE);
	FLHookSymbol(TileSource::$getData, TILESOURCE_GETDATA);
	FLHookSymbol(TileSource::$updateNeighborsAt, TILESOURCE_UPDATE);
	FLHookSymbol(TileSource::$getEntities, TILESOURCE_GETENTITIES);
	FLHookSymbol(TileSource::$tileEvent, TILESOURCE_TILEEVENT);
	FLHookSymbol(TileTessellator::$tessellateBlockInWorld, TILETESS_TESSBLOCK);
	FLHookSymbol(Tessellator::$vertex, TESS_VERTEX);
	FLHookSymbol(TileTickingQueue::$add, TICKQUEUE_ADD);
	FLHookSymbol(Item::creativeList, ITEM_CREATIVELIST);
	FLHookSymbol(DoorTile::$setOpen, DOORTILE_SETOPEN);

	FLHookFunction(TILETESS_TESS, MSHake(TileTessellator$tessellateInWorld));
	FLHookFunction(TILE_INITTILES, MSHake(Tile$initTiles));
	FLHookFunction(ITEM_USEON, MSHake(Item$useOn));
	FLHookFunction(ITEM_INITITEMS, MSHake(Item$initItems));
	FLHookFunction(ITEM_INITCREATIVE, MSHake(Item$initCreativeItems));
}