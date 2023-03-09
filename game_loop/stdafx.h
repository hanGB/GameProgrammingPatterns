#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <algorithm>

struct KeyInputs {
	bool left;
	bool right;
	bool up;
	bool down;
	bool spacebar;
};

#define MILLISECOND_PER_FRAME 16
#define NEED_EXP_LEVEL_COEFFICIENT 10
#define WORLD_WIDTH 21
#define WORLD_HEIGHT 11
#define LEVEL_UP_POP_UP_SHOWING_TIME 10
#define MAX_LEVEL 50

bool MoveObjectToWorld(int* x, int* y);