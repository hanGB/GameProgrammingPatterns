#include "stdafx.h"
#include "key_setting.h"

// 이동
PERKeyboardData KeySetting::MoveUpwards = { PERKeyboardValue::UP, L"↑"};
PERKeyboardData KeySetting::MoveDownwards = { PERKeyboardValue::DOWN, L"↓" };
PERKeyboardData KeySetting::MoveLeftwards = { PERKeyboardValue::LEFT, L"←" };
PERKeyboardData KeySetting::MoveRightwards = { PERKeyboardValue::RIGHT, L"→" };

// 정보
PERKeyboardData KeySetting::ShowInformation = { PERKeyboardValue::Q, L"Q" };
// 상호작용
PERKeyboardData KeySetting::Use = { PERKeyboardValue::F, L"F" };
// 공격
PERKeyboardData KeySetting::BulletAttack = { PERKeyboardValue::S, L"S" };
PERKeyboardData KeySetting::BladeAttack = { PERKeyboardValue::D, L"D" };
// 시스템
PERKeyboardData KeySetting::Pause = { PERKeyboardValue::ESC, L"ESC" };
