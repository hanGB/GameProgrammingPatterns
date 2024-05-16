#pragma once
#include "ui_element.h"


class TextViewer : public UiElement {
public:
	TextViewer();
	~TextViewer();

	void Init(PERVec2 pos, double fontSize, PERColor color, const wchar_t* text);

	void MatchWithData(PERVec2 pos);

	virtual void Update(PERController& controller, PERAudio& audio, double dTime);
	virtual void RenderOnScreen(PERRenderer& renderer, PERDatabase& database);
	virtual void RenderInWorld(PERRenderer& renderer, PERDatabase& database);


private:
	// 텍스트
	std::wstring m_text = L"TEXT";
};