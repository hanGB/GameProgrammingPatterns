#include "stdafx.h"
#include "key_setting.h"

// �̵�
PERKeyboardData KeySetting::MoveUpwards = { PERKeyboardValue::UP, L"��"};
PERKeyboardData KeySetting::MoveDownwards = { PERKeyboardValue::DOWN, L"��" };
PERKeyboardData KeySetting::MoveLeftwards = { PERKeyboardValue::LEFT, L"��" };
PERKeyboardData KeySetting::MoveRightwards = { PERKeyboardValue::RIGHT, L"��" };

// ����
PERKeyboardData KeySetting::ShowInformation = { PERKeyboardValue::Q, L"Q" };
// ��ȣ�ۿ�
PERKeyboardData KeySetting::Use = { PERKeyboardValue::F, L"F" };
// ����
PERKeyboardData KeySetting::BulletAttack = { PERKeyboardValue::S, L"S" };
PERKeyboardData KeySetting::BladeAttack = { PERKeyboardValue::D, L"D" };
