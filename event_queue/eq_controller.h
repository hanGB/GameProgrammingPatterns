#pragma once
#include <map>

enum class EqInputKeyValue {
	EQ_INPUT_W,
	EQ_INPUT_S,
	EQ_INPUT_A,
	EQ_INPUT_D,
	EQ_INPUT_E,
	EQ_INPUT_F,
	EQ_INPUT_SPACE,
	EQ_INPUT_SHIFT,
	EQ_INPUT_KEY_NUM
};

class EqController {
public:
	// ╫л╠шео
	static EqController& GetInstance();

	bool IsDown(EqInputKeyValue key) const;
	bool IsDownOnce(EqInputKeyValue key);
	bool IsDownMoreThanTime(EqInputKeyValue key, double time = KEY_LONG_PRESS_TIME);
	void UpdateInputData(WPARAM wParam, bool isDown, double elapsedTimeInSec);

	int GetXAxisDirection();
	int GetYAxisDirection();

private:
	EqController();
	~EqController();
	
	void InitKeyInformationData();
	void UpdateKeyValue(EqInputKeyValue key, bool isDown, double elapsedTimeInSec);

	std::map<EqInputKeyValue, bool> m_keyIsDownData;
	std::map<EqInputKeyValue, double> m_keyDownTimeData;
	std::map<EqInputKeyValue, bool> m_keyProcessedEndOnceData;

};