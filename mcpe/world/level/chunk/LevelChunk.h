#pragma once
#include "../TileTickingQueue.h"
#include <Forklift/Forklift.h>

#define CHUNK_FILLER FLArch(132, 132, 200)

class LevelChunk {
public:
	char filler[CHUNK_FILLER];
	TileTickingQueue tickingQueue;
};