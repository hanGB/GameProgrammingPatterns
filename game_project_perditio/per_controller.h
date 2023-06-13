#pragma once
#include <concurrent_unordered_map.h>

class PERController {
public:
	PERController();
	~PERController();

	// Ű���� �Է��� ���ȴ��� Ȯ��
	bool IsKeyboardPressed(PERKeyboardValue value);
	// Ű���� �Է��� ���� ���� ���ȴ���(�� ���� ó�� �ȵ� ���) Ȯ��
	bool IsKeyboardPressedRightNow(PERKeyboardValue value);
	// Ű���� �Է��� ���� ���� ���Ȱų�(�� ���� ó�� �ȵ� ���) Ư�� �ð����� ���� ���ȴ��� Ȯ��
	bool IsKeyboardPressedRightNowOrMoreThanTime(PERKeyboardValue value, double time = PER_KEYBOARD_LONG_PRESS_TIME);

	void SetKeyboardPressed(PERKeyboardValue value, bool pressed);

	void Update(double dTime);

private:
	void InitKeyboardInputDatas();

	// Ű���� �Է� ������ ���� ������
	concurrency::concurrent_unordered_map<PERKeyboardValue, bool> m_keyboardInputDatas;
	// Ű���� �Է� ������ ó���� �� ���� ���� ���� ������
	concurrency::concurrent_unordered_map<PERKeyboardValue, bool> m_keyboardInputProcessedNeverDatas;
	// Ű���� �Է��� �ð��� �󸶳� ���������� ���� ������
	concurrency::concurrent_unordered_map<PERKeyboardValue, double> m_keyboardInputPressTimerDatas;
};