#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#include <cmath>
#include <iostream>
#include <chrono>

// 디버그용 콘솔을 띄울 건지 결정
#define PER_DEBUG

// 기본 설정
#define PER_WINDOW_LOCATION_X 200
#define PER_WINDOW_LOCATION_Y 100
#define PER_WINDOW_WIDTH 1280
#define PER_WINDOW_HEIGHT 720

#define PER_MICROSEC_PER_UPDATE 8000
#define PER_MAXIMUM_UPDATE_LOOP_COUNT 4
#define PER_PIXEL_PER_METER 100

struct CoordinateData {
	double halfWidth;
	double halfHeight;

	void ConvertCoordinateOpenGLToWindows(double* x, double* y) {
		*x += halfWidth / (double)PER_PIXEL_PER_METER;
		*y *= -1;
		*y += halfHeight / (double)PER_PIXEL_PER_METER;
	}
	void ConvertCoordinateWindowsToOpenGL(double* x, double* y) {
		*x -= halfWidth;
		*y -= halfHeight;
		*y *= -1;

		*x /= (double)PER_PIXEL_PER_METER;
		*y /= (double)PER_PIXEL_PER_METER;
	}
};
