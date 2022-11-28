#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#include <cmath>
#include <iostream>
#include <list>

// 디버그용 콘솔을 띄울 건지 결정
#define DEBUG

#define WINDOW_SIZE 480
#define PIXEL_SIZE 20
#define GPP_PIXEL_PER_METER 100
#define GPP_TIME_PER_FRAME 16

struct GPPCoordinateData {
	float halfWidth;
	float halfHeight;

	void ConvertCoordinateWindowToOpenGL(float* x, float* y) {
		*x += halfWidth / (float)GPP_PIXEL_PER_METER;
		*y *= -1;
		*y += halfHeight / (float)GPP_PIXEL_PER_METER;
	}
};
