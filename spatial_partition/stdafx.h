#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#include <cmath>
#include <iostream>
#include <cassert>

// 디버그용 콘솔을 띄울 건지 결정
#define DEBUG

#define TIME_PER_FRAME 16
#define PIXEL_PER_METER 5
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define MAX_UNIT_COUNT 1024

struct CoordinateData {
	double halfWidth;
	double halfHeight;

	void ConvertCoordinateOpenGLToWindows(double* x, double* y) {
		*x += halfWidth / (double)PIXEL_PER_METER;
		*y *= -1;
		*y += halfHeight / (double)PIXEL_PER_METER;
	}
	void ConvertCoordinateWindowsToOpenGL(double* x, double* y) {
		*x -= halfWidth;
		*y -= halfHeight;
		*y *= -1;

		*x /= (double)PIXEL_PER_METER;
		*y /= (double)PIXEL_PER_METER;
	}
};

