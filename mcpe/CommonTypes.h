#pragma once

#include <stdint.h>

typedef uint_fast8_t DataID;
typedef uint_fast8_t FacingID;

struct TileID {
	unsigned char value;

	TileID() {
		this->value = 1;
	}

	TileID(unsigned char val) {
		this->value = val;
	}

	TileID(TileID const& other) {
		this->value = other.value;
	}

	bool operator==(char v) {
		return this->value == v;
	}

	bool operator==(int v) {
		return this->value == v;
	}

	bool operator==(TileID v) {
		return this->value == v.value;
	}

	TileID& operator=(const unsigned char& v) {
		this->value = v;
		return *this;
	}

	operator unsigned char() {
	return this->value;
	}
};

class FullTile {
public:
	TileID id;
	DataID data;

	FullTile(TileID id, DataID data) : id(id), data(data) {}
};