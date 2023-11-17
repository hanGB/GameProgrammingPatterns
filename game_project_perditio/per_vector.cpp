#include "stdafx.h"

double DistanceSquareAandB(PERVec3 a, PERVec3 b)
{
	double x = a.x - b.x;
	double y = a.y - b.y;
	double z = a.z - b.z;

	return x * x + y * y + z * z;
}

PERVec3 NormalizeVector(PERVec3 vec)
{
	double value = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	return PERVec3(vec.x / value, vec.y / value, vec.z / value);
}
