#pragma once

struct PERVec3 {
	double x;
	double y;
	double z;

	PERVec3() { x = 0.0, y = 0.0, z = 0.0; }
	PERVec3(double x, double y, double z) : x(x), y(y), z(z) {}
	PERVec3(PERVec3& rhs) : x(rhs.x), y(rhs.y), z(rhs.z) {}

	PERVec3 operator*(int rhs) {
		PERVec3 res;
		res.x = this->x * rhs;
		res.y = this->y * rhs;
		res.z = this->z * rhs;

		return res;
	}
};
struct PERVec2 {
	double x;
	double y;

	PERVec2() { x = 0.0, y = 0.0; }
	PERVec2(double x, double y) : x(x), y(y) {}
	PERVec2(PERVec2& rhs) : x(rhs.x), y(rhs.y) {}

	PERVec2 operator*(int rhs) {
		PERVec2 res;
		res.x = this->x * rhs;
		res.y = this->y * rhs;

		return res;
	}
};

struct PERColor {
	unsigned char r;
	unsigned char g;
	unsigned char b;

	PERColor() { r = 0, g = 0, b = 0; }
	PERColor(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b) {}
	PERColor(PERColor& rhs) : r(rhs.r), g(rhs.g), b(rhs.b) {}
};