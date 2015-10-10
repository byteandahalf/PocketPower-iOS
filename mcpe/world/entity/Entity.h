#pragma once
#include <vector>
#include <Forklift/Forklift.h>
class TileSource;
class Level;

#define ENTITY_SYNCHEDDATA FLArch(20, 20, 40)

class Entity {
public:
	void** vtable;
	char synchedData[ENTITY_SYNCHEDDATA];
	float x;
	float y;
	float z;
	char filler[12];
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
	char filler2[96];
	float heightOffset;
};

typedef std::vector<Entity*> EntityList;