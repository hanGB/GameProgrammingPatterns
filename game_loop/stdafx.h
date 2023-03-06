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
};

#define NEED_EXP_LEVEL_COEFFICIENT 10
#define WORLD_WIDTH 11
#define WORLD_HEIGHT 6
#define LEVEL_UP_POP_UP_SHOWING_TIME 10

void MoveCurser(int x, int y);
void MoveObjectToWorld(int* x, int* y);