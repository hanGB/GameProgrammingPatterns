#pragma once
#define PER_DEBUG

// ���� ���� ���̺귯��
#pragma comment(lib, "Msimg32.lib")

// �������� API 
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <tchar.h>
// ��Ƽ ������
#include <thread>
#include <mutex>
#include <atomic>
#include <ppl.h>
#include <concurrent_queue.h>
// �ܼ� �����
#include <iostream>
#include <string>
#include <stdarg.h>
// ������ Ÿ�� ����
#include <chrono>
// ���Ǽ�
#include <cmath>
#include <algorithm>
#include <vector>
#include <functional>

// �ʿ� ����ü
#include "per_vector.h"
#include "per_stat.h"

// �α�
#include "per_log.h"

// �⺻ ����
// ������ â ũ�� �� ��ġ
#define PER_DEFAULT_WINDOW_LOCATION_X 200
#define PER_DEFAULT_WINDOW_LOCATION_Y 100
#define PER_DEFAULT_WINDOW_WIDTH 1280
#define PER_DEFAULT_WINDOW_HEIGHT 720
#define PER_DEFAULT_WINDOW_BORDER_WIDTH 16
#define PER_DEFAULT_WINDOW_BAR_HEIGHT 39
#define PER_MAXIMUM_WINDOW_WIDTH 1800
#define PER_MAXIMUM_WINDOW_HEIGHT 1000

// ������
#define PER_NUM_WORKER_THREAD 7

// ���� ���̾�
#define PER_TRANSPARENT_COLOR RGB(1, 12, 123)

// ������Ʈ Ÿ�� ����
#define PER_MICROSEC_PER_UPDATE 8000
#define PER_MAXIMUM_UPDATE_LOOP_COUNT 4
// �ּ� ������ Ÿ��(�ִ� 240FPS)
#define PER_MINIMUM_FRAME_TIME 4000

// ȭ�� ����
#define PER_PIXEL_PER_METER 100
// ��Ʈ�ѷ� ����
#define PER_KEYBOARD_LONG_PRESS_TIME 0.5

// ���� ��
#define PER_GRAVITY 9.8
#define PER_FRICTION 0.7

// ��ü
#define PER_DEFAULT_OBJECT_POOL_SIZE  1024
#define PER_DEFAULT_MAX_OBJECTS		  1024

// �̺�Ʈ
#define PER_DEFAULT_MAX_EVENT_PENDING  512

// �ð�
// �ִ� ������ Ÿ��(100��)
#define PER_MAXIMUM_LIFE_TIME 3'153'600'000.0


enum class PERShapeType {
	// �߾� ��Ŀ
	ELLIPSE,
	RECTANGLE,
	ROUND_RECTANGLE,
	TRIANGLE,
	// left, top ��Ŀ
	ELLIPSE_WITH_LEFT_TOP_ANCHOR,
	RECTANGLE_WITH_LEFT_TOP_ANCHOR,
	ROUND_RECTANGLE_WITH_LEFT_TOP_ANCHOR,
	TRIANGLE_WITH_LEFT_TOP_ANCHOR,
	// ��
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
