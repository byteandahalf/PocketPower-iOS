#pragma once

const float PI = 3.14159265;

class Vec3 {
public:
	float x;
	float y;
	float z;

	Vec3() {}
	Vec3(float, float, float);

	void setComponents(float, float, float);
	void rotateAroundX(double);
	void rotateAroundY(double);
	void rotateAroundZ(double);
};