#pragma once
#include <concurrent_unordered_map.h>

class PERController {
public:
	PERController();
	~PERController();

	// 키보드 입력이 눌렸는지 확인
	bool IsKeyboardPressed(PERKeyboardValue value);
	// 키보드 입력이 지금 당장 눌렸는지(한 번도 처리 안된 경우) 확인
	bool IsKeyboardPressedRightNow(PERKeyboardValue value);
	// 키보드 입력이 지금 당장 눌렸거나(한 번도 처리 안된 경우) 특정 시간보다 오래 눌렸는지 확인
	bool IsKeyboardPressedRightNowOrMoreThanTime(PERKeyboardValue value, double time = PER_KEYBOARD_LONG_PRESS_TIME);

	void SetKeyboardPressed(PERKeyboardValue value, bool pressed);

	void Update(double dTime);

private:
	void InitKeyboardInputDatas();

	// 키보드 입력 정보에 대한 데이터
	concurrency::concurrent_unordered_map<PERKeyboardValue, bool> m_keyboardInputDatas;
	// 키보드 입력 정보를 처리된 적 없는 지에 대한 데이터
	concurrency::concurrent_unordered_map<PERKeyboardValue, bool> m_keyboardInputProcessedNeverDatas;
	// 키보드 입력의 시간이 얼마나 지났는지에 대한 데이터
	concurrency::concurrent_unordered_map<PERKeyboardValue, double> m_keyboardInputPressTimerDatas;
};