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
#define NEED_EXP_LEVEL_COEFFICIENT 5
#define WORLD_WIDTH 17
#define WORLD_HEIGHT 9
#define LEVEL_UP_POP_UP_SHOWING_TIME 2.0f
#define MAX_LEVEL 20
#define PLAYER_DEFAULT_SPEED 2.0f
#define PLAYER_SPEED_LEVEL_COEFFICIENT 1.0f

bool MoveObjectToWorld(int* x, int* y);