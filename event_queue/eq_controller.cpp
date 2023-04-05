#include "stdafx.h"
#include "eq_controller.h"

EqController& EqController::GetInstance()
{
	static EqController instance;
	return instance;
}

void EqController::UpdateInputData(WPARAM wParam, bool isDown, double g_elapsedTimeInSec)
{
	EqInputKeyValue key = EqInputKeyValue::EQ_INPUT_KEY_NUM;

	if (wParam == 'w' || wParam == 'W') key = EqInputKeyValue::EQ_INPUT_W;
	if (wParam == 'a' || wParam == 'A') key = EqInputKeyValue::EQ_INPUT_A;
	if (wParam == 's' || wParam == 'S') key = EqInputKeyValue::EQ_INPUT_S;
	if (wParam == 'd' || wParam == 'D') key = EqInputKeyValue::EQ_INPUT_D;
	if (wParam == 'e' || wParam == 'E') key = EqInputKeyValue::EQ_INPUT_E;
	if (wParam == 'f' || wParam == 'F') key = EqInputKeyValue::EQ_INPUT_F;

	if (wParam == VK_SPACE) key = EqInputKeyValue::EQ_INPUT_SPACE;
	if (wParam == VK_SHIFT) key = EqInputKeyValue::EQ_INPUT_SHIFT;

	if (key != EqInputKeyValue::EQ_INPUT_KEY_NUM)
		UpdateKeyValue(key, isDown, g_elapsedTimeInSec);
}

bool EqController::IsDown(EqInputKeyValue key) const
{
	return m_keyIsDownData.find(key)->second;
}

bool EqController::IsDownOnce(EqInputKeyValue key)
{
	if (m_keyIsDownData.find(key)->second) {
		if (!m_keyProcessedEndOnceData.find(key)->second) {
			m_keyProcessedEndOnceData.find(key)->second = true;
			m_keyDownTimeData.find(key)->second = 0.0;
			return true;
		}
	}
	return false;
}
bool EqController::IsDownMoreThanTime(EqInputKeyValue key, double time)
{
	if (m_keyIsDownData.find(key)->second) {
		if (m_keyDownTimeData.find(key)->second >= KEY_LONG_PRESS_TIME) {
			m_keyDownTimeData.find(key)->second = 0.0;
			return true;
		}
	}
	return false;
}

int EqController::GetXAxisDirection()
{
	bool inputA = m_keyIsDownData.find(EqInputKeyValue::EQ_INPUT_A)->second;
	bool inputD = m_keyIsDownData.find(EqInputKeyValue::EQ_INPUT_D)->second;

	if (inputA && !inputD) {
		return -1;
	}
	else if (!inputA && inputD) {
		return 1;
	}
	else {
		return 0;
	}
}

int EqController::GetYAxisDirection()
{
	bool inputS = m_keyIsDownData.find(EqInputKeyValue::EQ_INPUT_S)->second;
	bool inputW = m_keyIsDownData.find(EqInputKeyValue::EQ_INPUT_W)->second;

	if (inputS && !inputW) {
		return -1;
	}
	else if (!inputS && inputW) {
		return 1;
	}
	else {
		return 0;
	}
}

EqController::EqController()
{
	InitKeyInformationData();
}
EqController::~EqController()
{
}


void EqController::InitKeyInformationData()
{
	for (int i = 0; i < (int)EqInputKeyValue::EQ_INPUT_KEY_NUM; ++i) {
		m_keyIsDownData.insert(std::pair<EqInputKeyValue, bool>((EqInputKeyValue)i, false));
		m_keyDownTimeData.insert(std::pair<EqInputKeyValue, double>((EqInputKeyValue)i, 0.0));
		m_keyProcessedEndOnceData.insert(std::pair<EqInputKeyValue, bool>((EqInputKeyValue)i, false));
	}
}

void  EqController::UpdateKeyValue(EqInputKeyValue key, bool isDown, double elapsedTimeInSec)
{
	bool prev = IsDown(key);

	if (prev != isDown) {
		m_keyIsDownData.find(key)->second = isDown;
		m_keyProcessedEndOnceData.find(key)->second = false;
		m_keyDownTimeData.find(key)->second = 0.0;
	}
	else {
		m_keyDownTimeData.find(key)->second += elapsedTimeInSec;
	}
}