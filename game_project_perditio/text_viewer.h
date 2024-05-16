#pragma once
#include "ui_element.h"


class TextViewer : public UiElement {
public:
	TextViewer();
	~TextViewer();

	void Init(PERVec2 pos, double fontSize, PERColor color, const char* textId);

	void MatchWithData(PERVec2 pos);

	virtual void Update(PERController& controller, PERAudio& audio, double dTime);
	virtual void RenderOnScreen(PERRenderer& renderer, PERDatabase& database);
	virtual void RenderInWorld(PERRenderer& renderer, PERDatabase& database);


private:
	// 데이터 정보
	std::string m_textId;
	// 텍스트를 임시 저장하는 변수
	std::wstring m_textMemory;
};