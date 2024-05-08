#pragma once

struct PERVec3 {
	double x;
	double y;
	double z;

	PERVec3() { x = 0.0, y = 0.0, z = 0.0; }
	PERVec3(double x, double y, double z) : x(x), y(y), z(z) {}
	PERVec3(const PERVec3& copy) : x(copy.x), y(copy.y), z(copy.z) {}
	double GetSize() const {
		return std::sqrt(x * x + y * y + z * z);
	}

	PERVec3 operator*(double rhs) {
		PERVec3 res;
		res.x = this->x * rhs;
		res.y = this->y * rhs;
		res.z = this->z * rhs;

		return res;
	}
	PERVec3 operator+(const PERVec3& rhs) {
		PERVec3 res;
		res.x = this->x + rhs.x;
		res.y = this->y + rhs.y;
		res.z = this->z + rhs.z;

		return res;
	}
	PERVec3 operator-(const PERVec3& rhs) {
		PERVec3 res;
		res.x = this->x - rhs.x;
		res.y = this->y - rhs.y;
		res.z = this->z - rhs.z;

		return res;
	}

};
struct PERVec2 {
	double x;
	double y;

	PERVec2() { x = 0.0, y = 0.0; }
	PERVec2(double x, double y) : x(x), y(y) {}
	PERVec2(const PERVec2& copy) : x(copy.x), y(copy.y) {}
	double GetSize() const {
		return std::sqrt(x * x + y * y);
	}

	PERVec2 operator*(double rhs) {
		PERVec2 res;
		res.x = this->x * rhs;
		res.y = this->y * rhs;

		return res;
	}

	PERVec2 operator+(const PERVec2& rhs) {
		PERVec2 res;
		res.x = this->x + rhs.x;
		res.y = this->y + rhs.y;

		return res;
	}
	PERVec2 operator-(const PERVec2& rhs) {
		PERVec2 res;
		res.x = this->x - rhs.x;
		res.y = this->y - rhs.y;

		return res;
	}
};

struct PERColor {
	unsigned char r;
	unsigned char g;
	unsigned char b;

	PERColor() { r = 0, g = 0, b = 0; }
	PERColor(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b) {}
	PERColor(const PERColor& copy) : r(copy.r), g(copy.g), b(copy.b) {}
};

// 벡터 계산에 필요한 함수
// A와 B 사이의 거리의 제곱값
double DistanceSquareAandB(PERVec3 a, PERVec3 b);
// A와 B 사이의 거리의 제곱값(z값 무시)
double DistanceSquareAandBIgnoringZValue(PERVec3 a, PERVec3 b);
double DistanceSquareAandBIgnoringZValue(PERVec2 a, PERVec2 b);
PERVec3 NormalizeVector(PERVec3 vec);
