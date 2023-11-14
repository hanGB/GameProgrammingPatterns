#include "stdafx.h"

double DistanceSquareAandB(PERVec3 a, PERVec3 b)
{
	double x = a.x - b.x;
	double y = a.y - b.y;
	double z = a.z - b.z;

	return x * x + y * y + z * z;
}
