#pragma once
#include "stdafx.h"

enum class PERKeyboardValue {
	// special
	UP,
	DOWN,
	LEFT,
	RIGHT,
	SPACE,
	ESC,

	// alphabet
	A,
	S,
	D,
	F,
	Q,

	NUM_KEYBOARD_VALUE
};


struct PERKeyboardData {
	PERKeyboardValue value;
	std::wstring show;
};

class KeySetting {
public:
	// 이동
	static PERKeyboardData MoveUpwards;
	static PERKeyboardData MoveDownwards;
	static PERKeyboardData MoveLeftwards;
	static PERKeyboardData MoveRightwards;

	// 정보
	static PERKeyboardData ShowInformation;
	// 상호작용
	static PERKeyboardData Use;
	// 공격
	static PERKeyboardData BulletAttack;
	static PERKeyboardData BladeAttack;
	// 시스템
	static PERKeyboardData Pause;
};

