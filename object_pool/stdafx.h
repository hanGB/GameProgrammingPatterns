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
#define GRAVITATIONAL_ACCELERATION 9.8f
#define PIXEL_PER_METER 100
#define DEFUALT_PARTICLE_SIZE 0.2
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define PARTICLE_SPAWN_GAP 0.2

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

