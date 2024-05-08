#include "stdafx.h"

double DistanceSquareAandB(PERVec3 a, PERVec3 b)
{
	double x = a.x - b.x;
	double y = a.y - b.y;
	double z = a.z - b.z;

	return x * x + y * y + z * z;
}

double DistanceSquareAandBIgnoringZValue(PERVec3 a, PERVec3 b)
{
	double x = a.x - b.x;
	double y = a.y - b.y;

	return x * x + y * y;
}

double DistanceSquareAandBIgnoringZValue(PERVec2 a, PERVec2 b)
{
	double x = a.x - b.x;
	double y = a.y - b.y;

	return x * x + y * y;
}

PERVec3 NormalizeVector(PERVec3 vec)
{
	double value = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

	// 값이 0.0인 경우 처리
	if (value == 0.0) return PERVec3(0.0, 0.0, 0.0);
	return PERVec3(vec.x / value, vec.y / value, vec.z / value);
}
