#include "PistonArmTile.h"
#include "PistonBaseTile.h"
#include "../../../../mcpe/world/level/TileSource.h"
#include "../../../../mcpe/client/renderer/texture/TextureUVCoordinateSet.h"
#include "../../../../mcpe/world/Facing.h"
#include "../../../../mcpe/world/phys/AABB.h"
#include "../../../../addresses.h"

PistonArmTile::PistonArmTile(int blockId) : Tile(blockId, TextureUVCoordinateSet(0.0F, 0.0F, 0.0625F, 0.0625F), tiles[1]->material, "piston_extension") {
	init();
	
	destroyTime = 0.5F;
	renderType = 17;
	creativeTab = 4;
	Tile::solid[blockId] = false;
	Tile::lightBlock[blockId] = 0;

	tex = TextureUVCoordinateSet(0.6563F, 0.375F, 0.6875F, 0.4375F);
	texture_sticky = TextureUVCoordinateSet(0.625F, 0.375F, 0.6562F, 0.4375F);
	texture_side = TextureUVCoordinateSet(0.6875F, 0.375F, 0.7187F, 0.4375F);

	initVtable();
}

void PistonArmTile::initVtable() {
	vtable[VT_TILE_TEXTURE] = (void*) &getTexture;
	vtable[VT_TILE_GETSHAPEWORLD] = (void*) &getVisualShapeInWorld;
	vtable[VT_TILE_NEIGHBORCH] = (void*) &neighborChanged;
	vtable[VT_TILE_DESTROY] = (void*) &destroy;
	vtable[VT_TILE_ADDCOLLISION] = (void*) &addCollisionShapes;
}

void PistonArmTile::destroy(PistonArmTile* self, TileSource* region, int x, int y, int z, int side, Entity* player) {
	// Remove the attached piston base
	int data = region->getData(x, y, z);
	int rotation = Facing::OPPOSITE_FACING[getRotation(data)];
	x += Facing::STEP_X[rotation];
	y += Facing::STEP_Y[rotation];
	z += Facing::STEP_Z[rotation];
	int tile = region->getTile(x, y, z).id;
	if(tile == Tile::pistonNormal->id || tile == Tile::pistonSticky->id) {
		data = region->getData(x, y, z);
		if(PistonBaseTile::isPowered(data)) {
			region->setTileAndData(x, y, z, 0, 0, 0);
		}
	}
}

const AABB& PistonArmTile::getVisualShapeInWorld(PistonArmTile* self, TileSource* region, int x, int y, int z, AABB& shape, bool b) {
	switch(getRotation(region->getData(x, y, z))) {
	case 0:
		shape.set(0.0F, 0.0F, 0.0F, 1.0F, 0.25F, 1.0F);
		break;
	case 1:
		shape.set(0.0F, 0.75F, 0.0F, 1.0F, 1.0F, 1.0F);
		break;
	case 2:
		shape.set(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 0.25F);
		break;
	case 3:
		shape.set(0.0F, 0.0F, 0.75F, 1.0F, 1.0F, 1.0F);
		break;
	case 4:
		shape.set(0.0F, 0.0F, 0.0F, 0.25F, 1.0F, 1.0F);
		break;
	case 5:
		shape.set(0.75F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);
		break;
	}
	
	return shape;
}

void PistonArmTile::neighborChanged(PistonArmTile* self, TileSource* region, int x, int y, int z, int xx, int yy, int zz) {
	int rotation = getRotation(region->getData(x, y, z));
	Tile* tile = Tile::tiles[region->getTile(x - Facing::STEP_X[rotation], y - Facing::STEP_Y[rotation], z - Facing::STEP_Z[rotation]).id];
	if(tile == NULL)
		return;
	if(tile != Tile::pistonNormal && tile != Tile::pistonSticky)
		region->setTileAndData(x, y, z, 0, 0, 3);
	else {
		void (*neighborChanged)(Tile*, TileSource*, int, int, int, int, int, int) = (void (*)(Tile*, TileSource*, int, int, int, int, int, int)) tile->vtable[VT_TILE_NEIGHBORCH];
		neighborChanged(tile, region, x - Facing::STEP_X[rotation], y - Facing::STEP_Y[rotation], z - Facing::STEP_Z[rotation], xx, yy, zz);
	}
}

const TextureUVCoordinateSet& PistonArmTile::getTexture(PistonArmTile* self, signed char side, int data) {
	int rotation = getRotation(data);
	if(side == rotation)
		return isSticky(data)? self->texture_sticky : self->tex;
	int opposite[6] = {1, 0, 3, 2, 5, 4};
	if(side == opposite[rotation])
		return self->tex;
	return self->texture_side;
}

bool PistonArmTile::addCollisionShapes(PistonArmTile* self, TileSource& region, int x, int y, int z, AABB const* posAABB, std::vector<AABB>& pool) {
	int data = region.getData(x, y, z);
	float var9 = 0.25F;
	float var10 = 0.375F;
	float var11 = 0.625F;
	float var12 = 0.25F;
	float var13 = 0.75F;

	switch(getRotation(data)) {
    	case 0:
    	    pool.push_back(AABB(0.0F, 0.0F, 0.0F, 1.0F, 0.25F, 1.0F).move(x, y, z));
    	    pool.push_back(AABB(0.375F, 0.25F, 0.375F, 0.625F, 1.0F, 0.625F).move(x, y, z));
    	    break;
    	case 1:
    	    pool.push_back(AABB(0.0F, 0.75F, 0.0F, 1.0F, 1.0F, 1.0F).move(x, y, z));
    	    pool.push_back(AABB(0.375F, 0.0F, 0.375F, 0.625F, 0.75F, 0.625F).move(x, y, z));
    	    break;
    	case 2:
    	    pool.push_back(AABB(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 0.25F).move(x, y, z));
    	    pool.push_back(AABB(0.25F, 0.375F, 0.25F, 0.75F, 0.625F, 1.0F).move(x, y, z));
    	    break;
    	case 3:
    	    pool.push_back(AABB(0.0F, 0.0F, 0.75F, 1.0F, 1.0F, 1.0F).move(x, y, z));
    	    pool.push_back(AABB(0.25F, 0.375F, 0.0F, 0.75F, 0.625F, 0.75F).move(x, y, z));
    	    break;
    	case 4:
    	    pool.push_back(AABB(0.0F, 0.0F, 0.0F, 0.25F, 1.0F, 1.0F).move(x, y, z));
    	    pool.push_back(AABB(0.375F, 0.25F, 0.25F, 0.625F, 0.75F, 1.0F).move(x, y, z));
    	    break;
    	case 5:
    	    pool.push_back(AABB(0.75F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F).move(x, y, z));
    	    pool.push_back(AABB(0.0F, 0.375F, 0.25F, 0.75F, 0.625F, 0.75F).move(x, y, z));
    	    break;
    }
	return true;
}

int PistonArmTile::getRotation(int data) {
	return data & 7;
}

bool PistonArmTile::isSticky(int data) {
    int sticky = data & 8;
    if(sticky == 8) return true;
    if(sticky == 0) return false;
    return false;
}
