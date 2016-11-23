#include "Tile.h"
#include "../../item/TileItem.h"

Tile** Tile::tiles;
bool* Tile::solid;
bool* Tile::shouldTick;
void** Tile::_vtable;
void (*Tile::$init)(Tile*);
void (*Tile::$popResource)(Tile*, TileSource*, int, int, int, const ItemInstance&);
uint8_t* Tile::lightBlock;

Tile* Tile::redstoneDust;
Tile* Tile::pistonNormal;
Tile* Tile::pistonSticky;
Tile* Tile::pistonArm;
Tile* Tile::notGate_on;
Tile* Tile::notGate_off;
Tile* Tile::buttonStone;
Tile* Tile::buttonWood;
Tile* Tile::plateWood;
Tile* Tile::plateStone;
Tile* Tile::plateGold;
Tile* Tile::plateIron;
Tile* Tile::lever;
Tile* Tile::lamp_off;
Tile* Tile::lamp_on;
Tile* Tile::diode_off;
Tile* Tile::diode_on;


Tile::Tile(int id, TextureUVCoordinateSet texture, Material* material, const std::string& name) {
	vtable = (void**) malloc(VT_TILE_SIZE);
	memcpy(vtable, Tile::_vtable, VT_TILE_SIZE);
	replaceable = false;
	hitbox = AABB(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);
	tex = texture;
	this->id = id;
	soundType = tiles[1]->soundType;
	canBuildOver = false;
	renderPass = 2;
	renderType = 0;
	tileType = 0;
	tileEntityType = 0;
	thickness = tiles[1]->thickness;
	hasGravity = false;
	ticks = false;
	gravity = 1.0F;
	this->material = material;
	friction = tiles[1]->friction;
	destroyTime = 1.0F;
	explosionResistance = 1.0;
	creativeTab = 1;
	fancy = false;
	textureIsotropic = 0;
	this->name = name;
};

void Tile::init() {
	$init(this);
}

void Tile::popResource(TileSource* region, int x, int y, int z, const ItemInstance& item) {
	$popResource(this, region, x, y, z, item);
}