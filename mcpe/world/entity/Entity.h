#pragma once
#include <vector>
#include <Forklift/Forklift.h>
class TileSource;
class Level;

#define ENTITY_FILLER FLArch(44, 44, 64)

class Entity {
public:
	void** vtable;
	char filler[ENTITY_FILLER];
	TileSource& region;
	int added;
	int idk;
	Level* level;
	float oldX;
	float oldY;
	float oldZ;
	float velX;
	float velY;
	float velZ;
	float yaw;
};

typedef std::vector<Entity*> EntityList;