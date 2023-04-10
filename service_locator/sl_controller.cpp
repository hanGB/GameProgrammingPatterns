#include "stdafx.h"
#include "sl_controller.h"

SlController::SlController()
{
	InitKeyInformationData();
}
SlController::~SlController()
{
}


void SlController::UpdateInputData(WPARAM wParam, bool isDown, double g_elapsedTimeInSec)
{
	SlInputKeyValue key = SlInputKeyValue::NUM_OF_INPUT_KEY;

	if (wParam == 'w' || wParam == 'W') key = SlInputKeyValue::INPUT_W;
	if (wParam == 'a' || wParam == 'A') key = SlInputKeyValue::INPUT_A;
	if (wParam == 's' || wParam == 'S') key = SlInputKeyValue::INPUT_S;
	if (wParam == 'd' || wParam == 'D') key = SlInputKeyValue::INPUT_D;
	if (wParam == 'e' || wParam == 'E') key = SlInputKeyValue::INPUT_E;
	if (wParam == 'f' || wParam == 'F') key = SlInputKeyValue::INPUT_F;
	if (wParam == 'c' || wParam == 'C') key = SlInputKeyValue::INPUT_C;

	if (wParam == VK_SPACE) key = SlInputKeyValue::INPUT_SPACE;
	if (wParam == VK_SHIFT) key = SlInputKeyValue::INPUT_SHIFT;

	if (key != SlInputKeyValue::NUM_OF_INPUT_KEY)
		UpdateKeyValue(key, isDown, g_elapsedTimeInSec);
}

bool SlController::IsDown(SlInputKeyValue key) const
{
	return m_keyIsDownData.find(key)->second;
}

bool SlController::IsDownOnce(SlInputKeyValue key)
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
bool SlController::IsDownMoreThanTime(SlInputKeyValue key, double time)
{
	if (m_keyIsDownData.find(key)->second) {
		if (m_keyDownTimeData.find(key)->second >= SL_KEY_LONG_PRESS_TIME) {
			m_keyDownTimeData.find(key)->second = 0.0;
			return true;
		}
	}
	return false;
}

bool SlController::IsDownOnceOrMoreThanTIme(SlInputKeyValue key, double time)
{
	return IsDownOnce(key) || IsDownMoreThanTime(key, time);
}

int SlController::GetXAxisDirection()
{
	bool inputA = m_keyIsDownData.find(SlInputKeyValue::INPUT_A)->second;
	bool inputD = m_keyIsDownData.find(SlInputKeyValue::INPUT_D)->second;

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

int SlController::GetYAxisDirection()
{
	bool inputS = m_keyIsDownData.find(SlInputKeyValue::INPUT_S)->second;
	bool inputW = m_keyIsDownData.find(SlInputKeyValue::INPUT_W)->second;

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

void SlController::InitKeyInformationData()
{
	for (int i = 0; i < (int)SlInputKeyValue::NUM_OF_INPUT_KEY; ++i) {
		m_keyIsDownData.insert(std::pair<SlInputKeyValue, bool>((SlInputKeyValue)i, false));
		m_keyDownTimeData.insert(std::pair<SlInputKeyValue, double>((SlInputKeyValue)i, 0.0));
		m_keyProcessedEndOnceData.insert(std::pair<SlInputKeyValue, bool>((SlInputKeyValue)i, false));
	}
}

void  SlController::UpdateKeyValue(SlInputKeyValue key, bool isDown, double elapsedTimeInSec)
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