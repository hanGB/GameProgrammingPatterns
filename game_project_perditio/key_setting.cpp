#include "stdafx.h"
#include "key_setting.h"

// �̵�
PERKeyboardData KeySetting::MoveUpwards = { PERKeyboardValue::UP, (char)'��' };
PERKeyboardData KeySetting::MoveDownwards = { PERKeyboardValue::DOWN, (char)'��' };
PERKeyboardData KeySetting::MoveLeftwards = { PERKeyboardValue::LEFT, (char)'��' };
PERKeyboardData KeySetting::MoveRightwards = { PERKeyboardValue::RIGHT, (char)'��' };

// ����
PERKeyboardData KeySetting::ShowInformation = { PERKeyboardValue::Q, (char)'Q' };
// ��ȣ�ۿ�
PERKeyboardData KeySetting::Use = { PERKeyboardValue::F, (char)'F' };
// ����
PERKeyboardData KeySetting::BulletAttack = { PERKeyboardValue::S, (char)'S' };
PERKeyboardData KeySetting::BladeAttack = { PERKeyboardValue::D, (char)'D' };
