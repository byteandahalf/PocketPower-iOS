#pragma once
#include <vector>

class Entity {
public:
	void** vtable;
};

typedef std::vector<Entity*> EntityList;