#pragma once
#include <Forklift/Forklift.h>
#include <vector>
#include "../../../../vfp.h"

#define TESS_FILLER FLArch(100, 100, 140)

class Tessellator {
public:
	char filler[TESS_FILLER];
	float u, v;
	float u1, v1;
	unsigned int _color;

	static void (*$vertex)(Tessellator*, float, float, float) AAPCS_VFP;

	void vertexUV(float, float, float, float, float);
	void color(unsigned int);
};