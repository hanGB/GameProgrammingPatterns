#include "stdafx.h"
#include "per_controller.h"

PERController::PERController()
{
	PERLog::Logger().Info("��Ʈ�ѷ� ����");

	InitKeyboardInputDatas();
}

PERController::~PERController()
{
	PERLog::Logger().Info("��Ʈ�ѷ� ����");
}

bool PERController::IsKeyboardPressed(PERKeyboardValue value, bool isNotProcessed)
{
	// ó���� ������ ����(ó���� ������ ���� �ʴ� ��쵵 ����)
	if (!isNotProcessed) m_keyboardInputProcessedNeverDatas.find(value)->second = false;
	
	return m_keyboardInputDatas.find(value)->second;
}

bool PERController::IsKeyboardPressedRightNow(PERKeyboardValue value)
{
	// �ش� Ű�� �� ���� ó���� �� ���� �� Ȯ��
	if (!m_keyboardInputProcessedNeverDatas.find(value)->second) return false;

	// �ش� Ű�� ���ȴ� �� Ȯ��
	if (!m_keyboardInputDatas.find(value)->second) return false;
	m_keyboardInputProcessedNeverDatas.find(value)->second = false;

	return true;
}

bool PERController::IsKeyboardPressedRightNowOrMoreThanTime(PERKeyboardValue value, double time)
{
	// �ش� Ű�� ó���� �� ������ Ȯ��
	bool onceProcessed = IsKeyboardPressedRightNow(value);
	if (onceProcessed) return true;

	// �ش� Ű�� time���� �� ���ȴ��� Ȯ�� 
	if (m_keyboardInputPressTimerDatas.find(value)->second < time) return false;
	m_keyboardInputPressTimerDatas.find(value)->second = 0.0;

	return true;
}

void PERController::SetKeyboardPressed(PERKeyboardValue value, bool pressed)
{
	if (m_keyboardInputDatas.find(value)->second == pressed) return;

	// ���� ���� �ʱ�ȭ
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

