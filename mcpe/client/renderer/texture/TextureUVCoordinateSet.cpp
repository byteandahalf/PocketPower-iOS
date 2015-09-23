#include "TextureUVCoordinateSet.h"

TextureUVCoordinateSet::TextureUVCoordinateSet(float u0, float v0, float u1, float v1) {
	minU = u0;
	minV = v0;
	maxU = u1;
	maxV = v1;
	w = h = 16;
	file = 0;
	idk = 0;
}

float TextureUVCoordinateSet::getInterpolatedU(float inter) {
	float var3 = maxU - minU;
	return minU + var3 * inter / w;
}

float TextureUVCoordinateSet::getInterpolatedV(float inter) {
	float var3 = maxV - minV;
	return minV + var3 * inter / h;
}