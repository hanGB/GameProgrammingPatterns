#include "stdafx.h"
#include "per_controller.h"

PERController::PERController()
{
	PERLog::Logger().Info("컨트롤러 생성");

	InitKeyboardInputDatas();
}

PERController::~PERController()
{
	PERLog::Logger().Info("컨트롤러 삭제");
}

bool PERController::IsKeyboardPressed(PERKeyboardValue value, bool isNotProcessed)
{
	// 처리된 것으로 설정(처리된 것으로 하지 않는 경우도 존재)
	if (!isNotProcessed) m_keyboardInputProcessedNeverDatas.find(value)->second = false;
	
	return m_keyboardInputDatas.find(value)->second;
}

bool PERController::IsKeyboardPressedRightNow(PERKeyboardValue value)
{
	// 해당 키가 한 번도 처리된 적 없는 지 확인
	if (!m_keyboardInputProcessedNeverDatas.find(value)->second) return false;

	// 해당 키가 눌렸는 지 확인
	if (!m_keyboardInputDatas.find(value)->second) return false;
	m_keyboardInputProcessedNeverDatas.find(value)->second = false;

	return true;
}

bool PERController::IsKeyboardPressedRightNowOrMoreThanTime(PERKeyboardValue value, double time)
{
	// 해당 키가 처리된 적 없는지 확인
	bool onceProcessed = IsKeyboardPressedRightNow(value);
	if (onceProcessed) return true;

	// 해당 키가 time보다 더 눌렸는지 확인 
	if (m_keyboardInputPressTimerDatas.find(value)->second < time) return false;
	m_keyboardInputPressTimerDatas.find(value)->second = 0.0;

	return true;
}

void PERController::SetKeyboardPressed(PERKeyboardValue value, bool pressed)
{
	if (m_keyboardInputDatas.find(value)->second == pressed) return;

	// 각종 정보 초기화
	m_keyboardInputDatas.find(value)->second = pressed;
	m_keyboardInputProcessedNeverDatas.find(value)->second = true;
	m_keyboardInputPressTimerDatas.find(value)->second = 0.0;
}

void PERController::Update(double dTime)
{
	for (int i = 0; i < (int)PERKeyboardValue::NUM_KEYBOARD_VALUE; ++i) {
		if (m_keyboardInputDatas.find((PERKeyboardValue)i)->second)
			m_keyboardInputPressTimerDatas.find((PERKeyboardValue)i)->second += dTime;
	}
}

void PERController::InitKeyboardInputDatas()
{
	for (int i = 0; i < (int)PERKeyboardValue::NUM_KEYBOARD_VALUE; ++i) {
		m_keyboardInputDatas.insert(std::pair<PERKeyboardValue, bool>((PERKeyboardValue)i, false));
		m_keyboardInputProcessedNeverDatas.insert(std::pair<PERKeyboardValue, bool>((PERKeyboardValue)i, false));
		m_keyboardInputPressTimerDatas.insert(std::pair<PERKeyboardValue, double>((PERKeyboardValue)i, 0.0));
	}
}

