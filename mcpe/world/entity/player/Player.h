#pragma once
#include <Forklift/Forklift.h>
class TileSource;

#define PLAYER_FILLER FLArch(48, 48, 72)

struct Player {
	char filler[PLAYER_FILLER];
	TileSource& region;
};