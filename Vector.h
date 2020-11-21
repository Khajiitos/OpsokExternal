#pragma once

class Vector {
public:
	float x, y, z;

	Vector() : x(0.f), y(0.f), z(0.f) { };
	Vector(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { };
};