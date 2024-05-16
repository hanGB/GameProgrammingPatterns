#pragma once
#include "ui_element.h"


class SelectBox : public UiElement {
public:
	SelectBox();
	~SelectBox();

	void Init(PERVec2 pos, PERVec2 size, PERColor onColor, PERColor offColor, 
		bool onBorder, int onBorderWidth, PERColor onBorderColor,
		bool offBorder, int offBorderWidth, PERColor offBorderColor,
		bool text = false, std::string textId = "TEXT_ID", double fontSize = 0.2, PERVec2 textPos = PERVec2(0.0, 0.0),
		PERColor onTextColor = PERColor(255, 255, 255), PERColor offTextColor = PERColor(0, 0, 0));

	void MatchWithData(PERVec2 pos, PERVec2 textPos, bool on);

	virtual void Update(PERController& controller, PERAudio& audio, double dTime);
	virtual void RenderOnScreen(PERRenderer& renderer, PERDatabase& database);
	virtual void RenderInWorld(PERRenderer& renderer, PERDatabase& database);


private:
	bool m_isOn;

	PERColor	m_onColor;
	bool		m_onBorder;
	int		m_onBorderWidth;
	PERColor	m_onBorderColor;

	bool		m_isTextOn;
	double		m_fontSize;
	PERVec2		m_textPos;
	PERColor	m_onTextColor;
	PERColor	m_offTextColor;

	// 데이터 정보
	std::string m_textId;
	// 텍스트를 임시 저장하는 변수
	std::wstring m_textMemory;
};