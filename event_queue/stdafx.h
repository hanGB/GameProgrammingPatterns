#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>

#define DEBUG

#define KEY_LONG_PRESS_TIME 0.35

#define EQ_TIME_PER_FRAME 16
#define EQ_PIXEL_PER_METER 100

template<class T>
struct EqVector2 {
	T x;
	T y;
	T z;
};
template<class T>
struct EqVector3 {
	T x;
	T y;
	T z;
};

struct EqColor {
	unsigned char r;
	unsigned char g;
	unsigned char b;
};