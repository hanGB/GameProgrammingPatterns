#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <cassert>
#include <chrono>

#define SL_DEBUG

#define SL_KEY_LONG_PRESS_TIME 0.35

#define SL_TIME_PER_FRAME 16
#define SL_PIXEL_PER_METER 100
#define SL_BASE_WINDOW_WIDTH 640
#define SL_BASE_WINDOW_HEIGHT 360

#define SL_MAXIMUM_LOG_TEXT_SIZE 100

template<class T>
struct SlVector2 {
	T x;
	T y;
	T z;
};
template<class T>
struct SlVector3 {
	T x;
	T y;
	T z;
};

struct SlColor {
	unsigned char r;
	unsigned char g;
	unsigned char b;
};