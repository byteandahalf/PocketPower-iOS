#pragma once
#include <vector>
#include <Forklift/Forklift.h>
class TileSource;
class Level;

#define ENTITY_FILLER FLArch(44, 44, 64)

class Entity {
public:
	void** vtable; // 0
	char filler[ENTITY_FILLER]; // 8
	TileSource& region; // 72
	int added; // 80
	int idk; // 84
	Level* level; // 88
	float oldX; // 96
	float oldY; // 100
	float oldZ; // 104
	float velX; // 108
	float velY; // 112
	float velZ; // 116
	float yaw; // 120
	char filler2[96]; // 124
	float heightOffset; // 220
};

typedef std::vector<Entity*> EntityList;