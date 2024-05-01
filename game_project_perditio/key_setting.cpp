#include "stdafx.h"
#include "key_setting.h"

// 이동
PERKeyboardData KeySetting::MoveUpwards = { PERKeyboardValue::UP, (char)'↑' };
PERKeyboardData KeySetting::MoveDownwards = { PERKeyboardValue::DOWN, (char)'↓' };
PERKeyboardData KeySetting::MoveLeftwards = { PERKeyboardValue::LEFT, (char)'←' };
PERKeyboardData KeySetting::MoveRightwards = { PERKeyboardValue::RIGHT, (char)'→' };

// 정보
PERKeyboardData KeySetting::ShowInformation = { PERKeyboardValue::Q, (char)'Q' };
// 상호작용
PERKeyboardData KeySetting::Use = { PERKeyboardValue::F, (char)'F' };
// 공격
PERKeyboardData KeySetting::BulletAttack = { PERKeyboardValue::S, (char)'S' };
PERKeyboardData KeySetting::BladeAttack = { PERKeyboardValue::D, (char)'D' };
