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
// 수학
#include <cmath>

// 필요 구조체
#include "per_vector.h"

// 디버그용 콘솔을 띄울 건지 결정
#define PER_DEBUG

// 기본 설정
// 윈도우 창 크기 및 위치
#define PER_DEFAULT_WINDOW_LOCATION_X 200
#define PER_DEFAULT_WINDOW_LOCATION_Y 100
#define PER_DEFAULT_WINDOW_WIDTH 1280
#define PER_DEFAULT_WINDOW_HEIGHT 720
// 업데이트 타임 설정
#define PER_MICROSEC_PER_UPDATE 8000
#define PER_MAXIMUM_UPDATE_LOOP_COUNT 4

// 화면 설정
#define PER_PIXEL_PER_METER 100
// 컨트롤러 설정
#define PER_KEYBOARD_LONG_PRESS_TIME 0.5

// 물리 값
#define PER_GRAVITY 9.8
#define PER_FRICTION 0.7


enum class PERShapeType {
	SHAPE_TYPE_ELLIPSE,
	SHAPE_TYPE_RECTANGLE,
	SHAPE_TYPE_TRIANGLE,
	NUM_SHAPE_TYPE
};

enum class PERKeyboardValue {
	KEYBOARD_UP,
	KEYBOARD_DOWN,
	KEYBOARD_LEFT,
	KEYBOARD_RIGHT,
	KEYBOARD_SPACE,
	NUM_KEYBOARD_VALUE
};

enum class PERObjectType {
	OBJECT_TYPE_PLAYER,
	OBJECT_TYPE_FIXED,
	OBJECT_TYPE_MOVABLE,
	OBJECT_TYPE_UNINTELLIGENT,
	OBJECT_TYPE_INTELLIGENT,
	OBJECT_TYPE_INTERACT,
	OBJECT_TYPE_NO_INTERACT,
	OBJECT_TYPE_VISIBLE,
	OBJECT_TYPE_HIDDEN,
	NUM_OBJECT_TYPE
};
