#pragma once

struct TextureUVCoordinateSet {
	TextureUVCoordinateSet() {}
	TextureUVCoordinateSet(float, float, float, float);
	float getInterpolatedU(float);
	float getInterpolatedV(float);

	float minU, minV, maxU, maxV;
	int w, h, file, idk;
};