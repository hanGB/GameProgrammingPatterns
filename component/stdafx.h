#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>

#define DEBUG

#define COMP_TIME_PER_FRAME 16
#define COMP_PIXEL_PER_METER 100
#define GPP_GRAVITY 9.8f
#define GPP_FRICTION 0.7f

template<class T>
struct CompVector2 {
	T x;
	T y;
	T z;
};
template<class T>
struct CompVector3 {
	T x;
	T y;
	T z;
};

struct CompColor {
	unsigned char r;
	unsigned char g;
	unsigned char b;
};