#pragma once
// 윈도우즈 API 
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
// 멀티 스레드
#include <ppl.h>
// 콘솔 입출력
#include <iostream>
// 프레임 타임 측정
#include <chrono>

// 디버그용 콘솔을 띄울 건지 결정
#define PER_DEBUG

// 기본 설정
// 윈도우 창 크기 및 위치
#define PER_WINDOW_LOCATION_X 200
#define PER_WINDOW_LOCATION_Y 100
#define PER_WINDOW_WIDTH 1280
#define PER_WINDOW_HEIGHT 720
// 업데이트 타임 설정
#define PER_MICROSEC_PER_UPDATE 8000
#define PER_MAXIMUM_UPDATE_LOOP_COUNT 4
// 화면 설정
#define PER_PIXEL_PER_METER 100
// 컨트롤러 설정
#define PER_KEYBOARD_LONG_PRESS_TIME 0.5

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
