#pragma once
#include <map>

enum class SlInputKeyValue {
	INPUT_W,
	INPUT_S,
	INPUT_A,
	INPUT_D,
	INPUT_E,
	INPUT_F,
	INPUT_C,
	INPUT_L,
	INPUT_SPACE,
	INPUT_SHIFT,
	NUM_OF_INPUT_KEY
};

class SlController {
public:
	// ╫л╠шео
	SlController();
	~SlController();

	bool IsDown(SlInputKeyValue key) const;
	bool IsDownOnce(SlInputKeyValue key);
	bool IsDownMoreThanTime(SlInputKeyValue key, double time = SL_KEY_LONG_PRESS_TIME);
	bool IsDownOnceOrMoreThanTIme(SlInputKeyValue key, double time = SL_KEY_LONG_PRESS_TIME);
	void UpdateInputData(WPARAM wParam, bool isDown, double elapsedTimeInSec);

	int GetXAxisDirection();
	int GetYAxisDirection();

private:
	void InitKeyInformationData();
	void UpdateKeyValue(SlInputKeyValue key, bool isDown, double elapsedTimeInSec);

	std::map<SlInputKeyValue, bool> m_keyIsDownData;
	std::map<SlInputKeyValue, double> m_keyDownTimeData;
	std::map<SlInputKeyValue, bool> m_keyProcessedEndOnceData;

};