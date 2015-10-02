#include "PistonBaseTile.h"
#include "../../../../mcpe/world/level/TileSource.h"
#include "../../../../mcpe/client/renderer/texture/TextureUVCoordinateSet.h"
#include "../../../../mcpe/world/Facing.h"
#include "../../../../mcpe/world/phys/AABB.h"
#include "../../../../mcpe/world/entity/Mob.h"
#include "../../../../addresses.h"
#include <cmath>
#include <algorithm>


PistonBaseTile::PistonBaseTile(int blockId, bool sticky) : Tile(blockId, TextureUVCoordinateSet(0.0F, 0.0F, 0.0625F, 0.0625F), tiles[1]->material) {
	init();

	if(!sticky) {
		//setNameId("piston");
		tex = getTextureUVCoordinateSet(0.6563F, 0.375F, 0.6875F, 0.4375F);
	}
	else {
		//setNameId("piston_sticky");
		tex = getTextureUVCoordinateSet(0.625F, 0.375F, 0.6562F, 0.4375F);
	}

	this->sticky = sticky;
	creativeTab =  4;
	renderType = 0;
	destroyTime = 0.5F;
	solid[blockId] = false;
	lightBlock[blockId] = 0;

	texture_inner = getTextureUVCoordinateSet(0.7188F, 0.375F, 0.75F, 0.4375F);
	texture_bottom = getTextureUVCoordinateSet(0.75F, 0.375F, 0.7812F, 0.4375F);
	texture_side = getTextureUVCoordinateSet(0.6875F, 0.375F, 0.7187F, 0.4375F);

	initVtable();
}

void PistonBaseTile::initVtable() {
	vtable[VT_TILE_TEXTURE] = (void*) &getTexture;
	vtable[VT_TILE_TEXTUREWORLD] = (void*) &getTextureInWorld;
	vtable[VT_TILE_GETSHAPE] = (void*) &getVisualShape;
	vtable[VT_TILE_GETSHAPEWORLD] = (void*) &getVisualShapeInWorld;
	vtable[VT_TILE_NEIGHBORCH] = (void*) &neighborChanged;
	vtable[VT_TILE_ONPLACE] = (void*) &onPlace;
	vtable[VT_TILE_GETPLACEDATA] = (void*) &getPlacementDataValue;
	vtable[VT_TILE_EVENT] = (void*) &triggerEvent;
}

const TextureUVCoordinateSet& PistonBaseTile::getTexture(PistonBaseTile* self, signed char side, int data) {
	return (side == 1)? self->tex : self->texture_side;
}

const TextureUVCoordinateSet& PistonBaseTile::getTextureInWorld(PistonBaseTile* self, TileSource* region, int x, int y, int z, signed char side) {
	int data = region->getData(x, y, z);
	bool powered = isPowered(data);
	int rotation = getRotation(data);

	if(side == rotation)
		return (powered)? self->texture_inner : self->tex;
	int opposite[6] = {1, 0, 3, 2, 5, 4};
	if(side == opposite[rotation])
		return self->texture_bottom;

	return self->texture_side;
}

const AABB& PistonBaseTile::getVisualShape(PistonBaseTile* self, unsigned char data, AABB& shape, bool idk) {
	shape.set(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);
	return shape;
}

const AABB& PistonBaseTile::getVisualShapeInWorld(PistonBaseTile* self, TileSource* region, int x, int y, int z, AABB& shape, bool idk) {
	if(isPowered(region->getData(x, y, z))) {
		switch(getRotation(region->getData(x, y, z))) {
		case 0:
			shape.set(0.0F, 0.25F, 0.0F, 1.0F, 1.0F, 1.0F);
			break;
		case 1:
			shape.set(0.0F, 0.0F, 0.0F, 1.0F, 0.75F, 1.0F);
			break;
		case 2:
			shape.set(0.0F, 0.0F, 0.25F, 1.0F, 1.0F, 1.0F);
			break;
		case 3:
			shape.set(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 0.75F);
			break;
		case 4:
			shape.set(0.25F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);
			break;
		case 5:
			shape.set(0.0F, 0.0F, 0.0F, 0.75F, 1.0F, 1.0F);
			break;
		}
	}
	else
		shape.set(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);

	return shape;
}

int PistonBaseTile::getRotation(int data) {
	return data & 7;
}

bool PistonBaseTile::isPowered(int data) {
	return (data & 8) != 0;
}

void PistonBaseTile::neighborChanged(PistonBaseTile* self, TileSource* region, int x, int y, int z, int neighborX, int neighborY, int neighborZ) {
	self->updateState(region, x, y, z);
}

void PistonBaseTile::onPlace(PistonBaseTile* self, TileSource* region, int x, int y, int z) {
	self->updateState(region, x, y, z);
}

void PistonBaseTile::updateState(TileSource* region, int x, int y, int z) {
	int data = region->getData(x, y, z);
	int rotation = getRotation(data);
	if(data == 7)
		return;
	bool shouldBePowered = hasPower(region, x, y, z, rotation);
	if(shouldBePowered && !isPowered(data)) {
		if(canPushRow(region, x, y, z, rotation)) {
			region->setTileAndData(x, y, z, id, rotation | 8, 0);
			region->tileEvent(x, y, z, 0, rotation);
		}
	}
	else if(!shouldBePowered && isPowered(data)) {
		region->setTileAndData(x, y, z, id, rotation, 0);
		region->tileEvent(x, y, z, 1, rotation);
	}
}

bool PistonBaseTile::hasPower(TileSource* region, int x, int y, int z, int rotation) {
	if(rotation != 0 && region->getIndirectPowerOutput(x, y - 1, z, 0))
		return true;
	if(rotation != 1 && region->getIndirectPowerOutput(x, y + 1, z, 1))
		return true;
	if(rotation != 2 && region->getIndirectPowerOutput(x, y, z - 1, 2))
		return true;
	if(rotation != 3 && region->getIndirectPowerOutput(x, y, z + 1, 3))
		return true;
	if(rotation != 4 && region->getIndirectPowerOutput(x - 1, y, z, 4))
		return true;
	if(rotation != 5 && region->getIndirectPowerOutput(x + 1, y, z, 5))
		return true;

	// So BUD switches work
	if(region->getIndirectPowerOutput(x, y, z, 0) ||
		region->getIndirectPowerOutput(x, y + 2, z, 1) ||
		region->getIndirectPowerOutput(x, y + 1, z - 1, 2) ||
		region->getIndirectPowerOutput(x, y + 1, z + 1, 3) ||
		region->getIndirectPowerOutput(x - 1, y + 1, z, 4) ||
		region->getIndirectPowerOutput(x + 1, y + 1, z, 5))
			return true;
}

int PistonBaseTile::getPlacementDataValue(PistonBaseTile* self, Mob* player, int x, int y, int z, signed char side) {
	if(abs(player->x - x) < 2.0F && abs(player->z - z) < 2.0F) {
		float temp = player->y + 1.82F - player->heightOffset;
		if(temp - y > 2.0F)
			return 1;
		if(y - temp > 0.0F)
			return 0;
	}
	int temp2 = (int) floor(((player->yaw * 4.0F) / 360.0F) + 0.5F) & 3;
	return temp2 == 0 ? 2 : (temp2 == 1 ? 5 : (temp2 == 2 ? 3 : (temp2 == 3 ? 4 : 0)));
}

bool PistonBaseTile::canPushRow(TileSource* region, int x, int y, int z, int rotation) {
	int xx = x + Facing::STEP_X[rotation];
	int yy = y + Facing::STEP_Y[rotation];
	int zz = z + Facing::STEP_Z[rotation];
	int counter = 0;
	do {
		if(yy <= 0 || yy >= 127) break; // Can't push blocks out of the world
		int blockId = region->getTile(xx, yy, zz).id;
		if(blockId == 0 || Tile::tiles[blockId] == NULL)
			break;
		if(Tile::tiles[blockId]->replaceable) break; // Break things like flowers, etc.
		if(!isMoveableBlock(Tile::tiles[blockId], region, xx, yy, zz, true)) return false; // Can't extend the piston :(
			
		xx += Facing::STEP_X[rotation];
		yy += Facing::STEP_Y[rotation];
		zz += Facing::STEP_Z[rotation];
		counter++;
	} while(counter < 13); // You may only push 12 blocks

	return true;
}

bool PistonBaseTile::isMoveableBlock(Tile* tile, TileSource* region, int x, int y, int z, bool b) {
	if(tile->id == 49 || tile->id == 7 || tile->id == 34)
		return false;
	if(tile->id == 33 || tile->id == 29)
		return !isPowered(region->getData(x, y, z));
	
	return (tile->id != 52 && tile->id != 54 && tile->id != 61 && tile->id != 62 && tile->id != 63 && tile->id != 116 && tile->id != 117 && tile->id != 140 && tile->id != 143 && tile->id != 247);	
	//return region->getTileEntity({x, y, z}) == NULL;
}

void PistonBaseTile::triggerEvent(PistonBaseTile* self, TileSource* region, int x, int y, int z, int eventType, int rotation) {
	//self->ignoreUpdates = true;
	if(eventType == 0) {
		if(self->actuallyPushRow(region, x, y, z, rotation)) {
			region->setTileAndData(x, y, z, self->id, rotation | 8, 3);
			// play sound
		} else
			region->setTileAndData(x, y, z, self->id, rotation, 3);
	} else if(eventType == 1) {
		if(self->sticky) {
			int pullX = x + Facing::STEP_X[rotation] * 2;
			int pullY = y + Facing::STEP_Y[rotation] * 2;
			int pullZ = z + Facing::STEP_Z[rotation] * 2;
			int pullID = region->getTile(pullX, pullY, pullZ).id;
			int pullData = region->getData(pullX, pullY, pullZ);

			if(Tile::tiles[pullID] != NULL) {
				if(pullID > 0 && isMoveableBlock(Tile::tiles[pullID], region, pullX, pullY, pullZ, false) && !Tile::tiles[pullID]->replaceable) {
					region->setTileAndData(pullX - Facing::STEP_X[rotation], pullY - Facing::STEP_Y[rotation], pullZ - Facing::STEP_Z[rotation], pullID, pullData, 3);
					region->setTileAndData(pullX, pullY, pullZ, 0, 0, 3);
				}
			}
			else
				region->setTileAndData(pullX - Facing::STEP_X[rotation], pullY - Facing::STEP_Y[rotation], pullZ - Facing::STEP_Z[rotation], 0, 0, 3);
		} else {
			//self->ignoreUpdates = false;
			region->setTileAndData(x + Facing::STEP_X[rotation], y + Facing::STEP_Y[rotation], z + Facing::STEP_Z[rotation], 0, 0, 3);
			//self->ignoreUpdates = true;
		}
	}
	//self->ignoreUpdates = false;
}

bool PistonBaseTile::actuallyPushRow(TileSource* region, int x, int y, int z, int rotation) {
	int xx = x + Facing::STEP_X[rotation];
	int yy = y + Facing::STEP_Y[rotation];
	int zz = z + Facing::STEP_Z[rotation];
	int counter = 0;
	do {
		if(yy <= 0 || yy >= 127)
			break;
		int blockId = region->getTile(xx, yy, zz).id;
		if(blockId == 0 || Tile::tiles[blockId] == NULL)
			break;
		if(!isMoveableBlock(Tile::tiles[blockId], region, xx, yy, zz, true))
			return false;
		if(Tile::tiles[blockId]->replaceable)
			break;
			
		// TODO: Drop flowers and such
		if(counter == 12)
			return false;
			
		xx += Facing::STEP_X[rotation];
		yy += Facing::STEP_Y[rotation];
		zz += Facing::STEP_Z[rotation];
		counter++;
	} while(counter < 13);

	while(xx != x || yy != y || zz != z) {
		int i2 = xx - Facing::STEP_X[rotation];
		int k2 = yy - Facing::STEP_Y[rotation];
		int l2 = zz - Facing::STEP_Z[rotation];
		xx = i2;
		yy = k2;
		zz = l2;
		int pushID = region->getTile(xx, yy, zz).id;
		int pushData = region->getData(xx, yy, zz);
		if(pushID == id && i2 == x && k2 == y && l2 == z) {
			region->setTileAndData(xx + Facing::STEP_X[rotation], yy + Facing::STEP_Y[rotation], zz + Facing::STEP_Z[rotation], 34, rotation | (sticky? 8 : 0), 3);
		} else {
			region->setTileAndData(xx + Facing::STEP_X[rotation], yy + Facing::STEP_Y[rotation], zz + Facing::STEP_Z[rotation], pushID, pushData, 3);
		}
	}
	//region>setTileAndData(xx + Facing::STEP_X[rotation], yy + Facing::STEP_Y[rotation], zz + Facing::STEP_Z[rotation], 34, rotation, 0);
	return true;
}
