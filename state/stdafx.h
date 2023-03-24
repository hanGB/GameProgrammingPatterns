#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#include <cmath>
#include <iostream>
#include <list>

// 디버그용 콘솔을 띄울 건지 결정
#define DEBUG

#define GPP_TIME_PER_FRAME 16
#define GPP_GRAVITY 9.8f
#define GPP_FRICTION 0.7f
#define GPP_PIXEL_PER_METER 100
#define MAXIMUM_RGB_BRIGHTNESS 255

struct GPPInputChunk {
	bool Input_W = false;
	bool Input_A = false;
	bool Input_S = false;
	bool Input_D = false;
	bool Input_Space = false;
};

struct GPPCoordinateData {
	float halfWidth;
	float halfHeight;

	void ConvertCoordinateOpenGLToWindows(float* x, float* y) {
		*x += halfWidth / (float)GPP_PIXEL_PER_METER;
		*y *= -1;
		*y += halfHeight / (float)GPP_PIXEL_PER_METER;
	}
};
