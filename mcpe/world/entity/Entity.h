#pragma once
#include <vector>
#include <Forklift/Forklift.h>
class TileSource;

#define ENTITY_FILLER FLArch(44, 44, 64)

class Entity {
public:
	void** vtable;
	char filler[ENTITY_FILLER];
	TileSource& region;
};

typedef std::vector<Entity*> EntityList;