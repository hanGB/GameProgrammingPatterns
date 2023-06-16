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
// ����
#include <cmath>

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
