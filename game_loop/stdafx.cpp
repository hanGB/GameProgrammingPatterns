#include "stdafx.h"

bool MoveObjectToWorld(int* x, int* y)
{
	// object can not move on wall
	// so calculate with size 1 down

	int originalX = *x, originalY = *y;

	*x = std::clamp(*x, -WORLD_WIDTH + 1, WORLD_WIDTH - 1);
	*y = std::clamp(*y, -WORLD_HEIGHT + 1, WORLD_HEIGHT - 1);

	// check collidide with wall
	if (originalX != *x || originalY != *y) return true;
	return false;
}
