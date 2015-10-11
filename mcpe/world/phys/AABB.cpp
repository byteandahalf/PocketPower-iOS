#include "AABB.h"

AABB::AABB(float f1, float f2, float f3, float f4, float f5, float f6) {
	this->f1 = f1;
	this->f2 = f2;
	this->f3 = f3;
	this->f4 = f4;
	this->f5 = f5;
	this->f6 = f6;
}

void AABB::set(float f1, float f2, float f3, float f4, float f5, float f6) {
	this->f1 = f1;
	this->f2 = f2;
	this->f3 = f3;
	this->f4 = f4;
	this->f5 = f5;
	this->f6 = f6;
}

AABB& AABB::move(float movX, float movY, float movZ) {
	f1 += movX;
	f2 += movY;
	f3 += movZ;
	f4 += movX;
	f5 += movY;
	f6 += movZ;
	return *this;
}