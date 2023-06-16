#pragma once
// �������� API 
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
// ��Ƽ ������
#include <ppl.h>
// �ܼ� �����
#include <iostream>
// ������ Ÿ�� ����
#include <chrono>
// ���Ǽ�
#include <cmath>
#include <algorithm>
// �⺻ ����ü
#include <vector>

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
// ������Ʈ Ÿ�� ����
#define PER_MICROSEC_PER_UPDATE 8000
#define PER_MAXIMUM_UPDATE_LOOP_COUNT 4

// ȭ�� ����
#define PER_PIXEL_PER_METER 100
// ��Ʈ�ѷ� ����
#define PER_KEYBOARD_LONG_PRESS_TIME 0.5

// ���� ��
#define PER_GRAVITY 9.8
#define PER_FRICTION 0.7

// ��ü
#define PER_DEFAULT_OBJECT_POOL_SIZE 1024
#define PER_DEFAULT_MAX_OBJECTS		  1024


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

enum class PERComponentType {
	COMPONENT_TYPE_PLAYER,
	COMPONENT_TYPE_FIXED,
	COMPONENT_TYPE_MOVABLE,
	COMPONENT_TYPE_UNINTELLIGENT,
	COMPONENT_TYPE_INTELLIGENT,
	COMPONENT_TYPE_INTERACT,
	COMPONENT_TYPE_NO_INTERACT,
	COMPONENT_TYPE_VISIBLE,
	COMPONENT_TYPE_HIDDEN,
	NUM_COMPONENT_TYPE
};

enum class PERObjectType {
	OBJECT_TYPE_PLAYER,
	OBJECT_TYPE_BLOCK,
	OBJECT_TYPE_MONSTER,
	OBJECT_TYPE_SWITCH,
	NUM_OBJECT_TYPE
};
