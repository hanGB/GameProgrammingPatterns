#pragma once

struct PERKeyboardData {
	PERKeyboardValue value;
	char show;
};

class KeySetting {
public:
	// �̵�
	static PERKeyboardData MoveUpwards;
	static PERKeyboardData MoveDownwards;
	static PERKeyboardData MoveLeftwards;
	static PERKeyboardData MoveRightwards;

	// ����
	static PERKeyboardData ShowInformation;
	// ��ȣ�ۿ�
	static PERKeyboardData Use;
	// ����
	static PERKeyboardData BulletAttack;
	static PERKeyboardData BladeAttack;
};

