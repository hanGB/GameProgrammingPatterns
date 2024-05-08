#pragma once
#include "ui_element.h"


class NameTag : public UiElement {
public:
	NameTag();
	~NameTag();

	void Init(PERVec2 pos, PERVec2 size, PERColor background, std::string nameId, double fontSize, PERVec2 textPos, PERColor textColor);

	void MatchWithData(PERVec2 pos, PERVec2 textPos);

	virtual void Update(PERController& controller, PERAudio& audio, double dTime);
	virtual void RenderOnScreen(PERRenderer& renderer, PERDatabase& database);
	virtual void RenderInWorld(PERRenderer& renderer, PERDatabase& database);


private:
	// 모양 정보
	double		m_fontSize;
	PERVec2		m_textPosition;
	PERColor	m_textColor;

	// 데이터 정보
	std::string m_nameId;
	// 이름 텍스트를 임시 저장하는 변수
	std::wstring m_textMemory;
};