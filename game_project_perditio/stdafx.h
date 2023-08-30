#pragma once
#define PER_DEBUG

// 투명 관련 라이브러리
#pragma comment(lib, "Msimg32.lib")

// 윈도우즈 API 
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <tchar.h>
// 멀티 스레드
#include <thread>
#include <mutex>
#include <atomic>
#include <ppl.h>
#include <concurrent_queue.h>
// 콘솔 입출력
#include <iostream>
#include <string>
#include <stdarg.h>
// 프레임 타임 측정
#include <chrono>
// 편의성
#include <cmath>
#include <algorithm>
#include <vector>
#include <functional>

// 필요 구조체
#include "per_vector.h"
#include "per_stat.h"

// 로그
#include "per_log.h"

// 기본 설정
// 윈도우 창 크기 및 위치
#define PER_DEFAULT_WINDOW_LOCATION_X 200
#define PER_DEFAULT_WINDOW_LOCATION_Y 100
#define PER_DEFAULT_WINDOW_WIDTH 1280
#define PER_DEFAULT_WINDOW_HEIGHT 720
#define PER_DEFAULT_WINDOW_BORDER_WIDTH 16
#define PER_DEFAULT_WINDOW_BAR_HEIGHT 39
#define PER_MAXIMUM_WINDOW_WIDTH 1800
#define PER_MAXIMUM_WINDOW_HEIGHT 1000

// 스레드
#define PER_NUM_WORKER_THREAD 7

// 투명 레이어
#define PER_TRANSPARENT_COLOR RGB(1, 12, 123)

// 업데이트 타임 설정
#define PER_MICROSEC_PER_UPDATE 8000
#define PER_MAXIMUM_UPDATE_LOOP_COUNT 4
// 최소 프레임 타임(최대 240FPS)
#define PER_MINIMUM_FRAME_TIME 4000

// 화면 설정
#define PER_PIXEL_PER_METER 100
// 컨트롤러 설정
#define PER_KEYBOARD_LONG_PRESS_TIME 0.5

// 물리 값
#define PER_GRAVITY 9.8
#define PER_FRICTION 0.7

// 객체
#define PER_DEFAULT_OBJECT_POOL_SIZE  1024
#define PER_DEFAULT_MAX_OBJECTS		  1024

// 이벤트
#define PER_DEFAULT_MAX_EVENT_PENDING  512

// 시간
// 최대 라이프 타임(100년)
#define PER_MAXIMUM_LIFE_TIME 3'153'600'000.0


enum class PERShapeType {
	// 중앙 앵커
	ELLIPSE,
	RECTANGLE,
	ROUND_RECTANGLE,
	TRIANGLE,
	// left, top 앵커
	ELLIPSE_WITH_LEFT_TOP_ANCHOR,
	RECTANGLE_WITH_LEFT_TOP_ANCHOR,
	ROUND_RECTANGLE_WITH_LEFT_TOP_ANCHOR,
	TRIANGLE_WITH_LEFT_TOP_ANCHOR,
	// 수
	NUM_SHAPE_TYPE
};

enum class PERBoundingType {
	RECTANGLE,
	CIRCLE,
	TRIANGLE,
	
	NUM_BOUNDING_TYPE
};

enum class PERKeyboardValue {
	// special
	UP,
	DOWN,
	LEFT,
	RIGHT,
	SPACE,

	// alphabet
	A,
	S,
	D,
	F,

	NUM_KEYBOARD_VALUE
};

enum class PERComponentType {
	// input
	PLAYER_INPUT,
	INTERACT,
	NO_INTERACT,

	// ai
	UNINTELLIGENT,
	INTELLIGENT,

	// pysics
	FIXED,
	MOVABLE,

	// graphics
	VISIBLE,
	HIDDEN,

	NUM_COMPONENT_TYPE
};

enum class PERObjectType {
	PLAYER,
	FIXED_BLOCK,
	MOVABLE_BLOCK,
	MONSTER,
	BULLET,
	TRIGGER,
	NUM_OBJECT_TYPE
};

enum class PERObjectStateType {
	PLAYER,
	MONSTER,
	NON,
	NUM_OBJECT_STATE_TYPE
};
