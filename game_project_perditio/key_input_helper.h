#pragma once
#include "ui_element.h"
#include "key_setting.h"

class KeyInputHelper : public UiElement {
public:
	KeyInputHelper();
	~KeyInputHelper();

	void Init(PERVec2 pos, PERVec2 size, PERColor defaultColor, PERColor pressedColor, 
		PERKeyboardData key, double fontSize, PERColor defaultTextColor, PERColor pressedTextColor);

	void MatchWithData(PERVec2 pos);

	virtual void Update(PERController& controller, PERAudio& audio, double dTime);
	virtual void RenderOnScreen(PERRenderer& renderer, PERDatabase& database);
	virtual void RenderInWorld(PERRenderer& renderer, PERDatabase& database);

private:
	// 버튼이 눌렸을 때의 컬러
	PERColor m_pressedColor;

	// 표시할 키의 정보
	PERKeyboardData m_key;
	double m_fontSize;
	PERColor m_defaultTextColor;
	PERColor m_pressedTextColor;

	// 키가 눌렸는지 여부
	bool m_isPressed;
};