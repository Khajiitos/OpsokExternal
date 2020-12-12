#pragma once
#include <math.h>

class Vector {
public:
	float x, y, z;

	Vector() : x(0.f), y(0.f), z(0.f) { };
	Vector(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { };

	void normalize() {

		while (y > 180)
			y -= 360;
		while (y < -180)
			y += 360;

		if (x > 89.f)
			x = 89.f;
		else if (x < -89.f)
			x = -89.f;

		z = 0.f;
	}

	Vector operator-=(Vector other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	Vector operator-(Vector other) {
		Vector vec(x, y, z);
		vec.x -= other.x;
		vec.y -= other.y;
		vec.z -= other.z;
		return vec;
	}

	Vector operator+=(Vector other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vector operator+(Vector other) {
		Vector vec(x, y, z);
		vec.x += other.x;
		vec.y += other.y;
		vec.z += other.z;
		return vec;
	}

	Vector operator*=(float mul) {
		x *= mul;
		y *= mul;
		z *= mul;
		return *this;
	}

	Vector operator*(float mul) {
		Vector vec(x, y, z);
		vec.x *= mul;
		vec.y *= mul;
		vec.z *= mul;
		return vec;
	}

	Vector operator/=(Vector other) {
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	Vector operator/(Vector other) {
		Vector vec(x, y, z);
		vec.x /= other.x;
		vec.y /= other.y;
		vec.z /= other.z;
		return vec;
	}

	bool zero() {
		return !x && !y && !z;
	}

	float magnitude() {
		return sqrtf(x * x + y * y + z * z);
	}

	float distance(Vector other) {
		Vector diff = *this - other;
		return diff.magnitude();
	}
};
