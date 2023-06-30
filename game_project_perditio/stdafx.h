#pragma once
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
// ������ Ÿ�� ����
#include <chrono>
// ���Ǽ�
#include <cmath>
#include <algorithm>
#include <vector>
#include <functional>

// �ʿ� ����ü
#include "per_vector.h"

// ����׿� �ܼ��� ��� ���� ����
#define PER_DEBUG

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
	SHAPE_TYPE_ELLIPSE,
	SHAPE_TYPE_RECTANGLE,
	SHAPE_TYPE_ROUND_RECTANGLE,
	SHAPE_TYPE_TRIANGLE,
	// left, top ��Ŀ
	SHAPE_TYPE_ELLIPSE_WITH_LEFT_TOP_ANCHOR,
	SHAPE_TYPE_RECTANGLE_WITH_LEFT_TOP_ANCHOR,
	SHAPE_TYPE_ROUND_RECTANGLE_WITH_LEFT_TOP_ANCHOR,
	SHAPE_TYPE_TRIANGLE_WITH_LEFT_TOP_ANCHOR,
	// ��
	NUM_SHAPE_TYPE
};

enum class PERKeyboardValue {
	// special
	KEYBOARD_UP,
	KEYBOARD_DOWN,
	KEYBOARD_LEFT,
	KEYBOARD_RIGHT,
	KEYBOARD_SPACE,

	// alphabet
	KEYBOARD_A,
	KEYBOARD_S,
	KEYBOARD_D,
	KEYBOARD_F,

	NUM_KEYBOARD_VALUE
};

enum class PERComponentType {
	// input
	COMPONENT_TYPE_PLAYER_INPUT,
	COMPONENT_TYPE_INTERACT,
	COMPONENT_TYPE_NO_INTERACT,

	// ai
	COMPONENT_TYPE_UNINTELLIGENT,
	COMPONENT_TYPE_INTELLIGENT,

	// pysics
	COMPONENT_TYPE_FIXED,
	COMPONENT_TYPE_MOVABLE,

	// graphics
	COMPONENT_TYPE_VISIBLE,
	COMPONENT_TYPE_HIDDEN,

	NUM_COMPONENT_TYPE
};

enum class PERObjectType {
	OBJECT_TYPE_PLAYER,
	OBJECT_TYPE_BLOCK,
	OBJECT_TYPE_MONSTER,
	OBJECT_TYPE_BULLET,
	OBJECT_TYPE_SWITCH,
	NUM_OBJECT_TYPE
};

