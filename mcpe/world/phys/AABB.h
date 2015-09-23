#pragma once

struct AABB {
	AABB(){}
	AABB(float, float, float, float, float, float);
	void set(float, float, float, float, float, float);

	int valid;
	float f1, f2, f3, f4, f5, f6;
};