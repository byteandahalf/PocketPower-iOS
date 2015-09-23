#pragma once

struct TilePos {
	TilePos(int x, int y, int z) : x(x), y(y), z(z) {}
	int x, y, z;
};