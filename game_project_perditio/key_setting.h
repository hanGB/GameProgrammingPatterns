#pragma once

struct PERKeyboardData {
	PERKeyboardValue value;
	char show;
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
};

