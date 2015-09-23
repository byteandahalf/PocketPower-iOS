#include "Tessellator.h"

void (*Tessellator::$vertex)(Tessellator*, float, float, float);

void Tessellator::vertexUV(float x, float y, float z, float u, float v) {
	this->u = u;
	this->v = v;
	$vertex(this, x, y, z);
}

void Tessellator::color(unsigned int color) {
	int b = 0xFF & (color >> 0);
	int g = 0xFF & (color >> 8);
	int r = 0xFF & (color >> 16);
	int a = 0xFF & (color >> 24);
	_color = (a << 24 | b << 16 | g << 8 | r << 0);
}