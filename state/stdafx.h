#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#include <cmath>
#include <iostream>

// 디버그용 콘솔을 띄울 건지 결정
//#define DEBUG

#define MAXIM_OBJECT_COUNT 100
#define GPP_TIME_PER_FRAME 16
#define GPP_GRAVITY 9.8f
#define GPP_FRICTION 0.7f
#define GPP_PIXEL_PER_METER 100

struct GPPInputChunk {
	bool Input_W = false;
	bool Input_A = false;
	bool Input_S = false;
	bool Input_D = false;
};

struct GPPCoordinateData {
	float halfWidth;
	float halfHeight;

	void ConvertCoordinateWindowToOpenGL(float* x, float* y) {
		*x += halfWidth / (float)GPP_PIXEL_PER_METER;
		*y *= -1;
		*y += halfHeight / (float)GPP_PIXEL_PER_METER;
	}
};

enum class GPPGameObjectType {
	PLAYER,
	FIXED,
	MOVABLE
};

enum class GPPGameObjectState {
	STANDING_ON_GROUND,
	FALLING
};
