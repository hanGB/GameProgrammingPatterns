#include "stdafx.h"
#include "select_box.h"
#include "per_renderer.h"
#include "per_database.h"

SelectBox::SelectBox()
{
}

SelectBox::~SelectBox()
{
}

void SelectBox::Init(PERVec2 pos, PERVec2 size, 
	bool onBackground, PERColor onColor, bool offBackground, PERColor offColor,
	bool onBorder, int onBorderWidth, PERColor onBorderColor,
	bool offBorder, int offBorderWidth, PERColor offBorderColor,
	bool text, std::string textId, double fontSize, PERVec2 textPos, PERColor onTextColor, PERColor offTextColor)
{
	m_isOn = false;

	m_onBackground = onBackground;
	m_offBackground = offBackground;

	m_onColor = onColor;
	m_onBorder = onBorder;
	m_onBorderWidth = onBorderWidth;
	m_onBorderColor = onBorderColor;

	m_isTextOn = text;
	m_textId = textId;
	m_fontSize = fontSize;
	m_textPos = textPos;
	m_onTextColor = onTextColor;
	m_offTextColor = offTextColor;

	UiElement::Init(pos, size, offColor, offBorder, offBorderWidth, offBorderColor);
}

void SelectBox::MatchWithData(PERVec2 pos, PERVec2 textPos, bool on)
{
	SetPosition(pos);
	m_textPos = textPos;
	m_isOn = on;

}

void SelectBox::Update(PERController& controller, PERAudio& audio, double dTime)
{
	if (!GetIsInUse()) return;
}

void SelectBox::RenderOnScreen(PERRenderer& renderer, PERDatabase& database)
{
	if (!GetIsInUse()) return;

	// 박스
	bool		background = m_offBackground;
	PERColor	color = GetBackgroundColor();
	bool		border = GetBorder();
	int			borderWidth = GetBorderWidth();
	PERColor	borderColor = GetBorderColor();

	if (m_isOn)
	{
		background = m_onBackground;
		color = m_onColor;
		border = m_onBorder;
		borderWidth = m_onBorderWidth;
		borderColor = m_onBorderColor;
	}	
	if (background) 
	{
		renderer.RenderShapeInScreenCoordinate(PERShapeType::RECTANGLE_WITH_LEFT_TOP_ANCHOR,
			GetPosition(), GetSize(), color, border, borderWidth, borderColor);
	}
	// 텍스트
	if (!m_isTextOn) return;

	PERColor textColor = m_offTextColor;
	if (m_isOn)
	{
		textColor = m_onTextColor;
	}
	TranslateData* data = database.GetTranslateData(m_textId.c_str());
	m_textMemory = data->korKR.c_str();
	renderer.RenderFontInScreenCoordinate(m_textMemory.c_str(), (int)m_textMemory.size(), m_fontSize, m_textPos, textColor);
}

void SelectBox::RenderInWorld(PERRenderer& renderer, PERDatabase& database)
{
	if (!GetIsInUse()) return;

	// 박스
	bool		background = m_offBackground;
	PERColor	color = GetBackgroundColor();
	bool		border = GetBorder();
	int			borderWidth = GetBorderWidth();
	PERColor	borderColor = GetBorderColor();

	if (m_isOn)
	{
		background = m_onBackground;
		color = m_onColor;
		border = m_onBorder;
		borderWidth = m_onBorderWidth;
		borderColor = m_onBorderColor;
	}
	if (background)
	{
		renderer.RenderShapeInWorldCoordinate(PERShapeType::RECTANGLE_WITH_LEFT_TOP_ANCHOR,
			PERVec3(GetPosition().x, GetPosition().y, 0.0), PERVec3(GetSize().x, GetSize().y, 0.0),
			color, border, borderWidth, borderColor);
	}
	
	// 텍스트
	if (!m_isTextOn) return;

	PERColor textColor = m_offTextColor;
	if (m_isOn)
	{
		textColor = m_onTextColor;
	}
	TranslateData* data = database.GetTranslateData(m_textId.c_str());
	m_textMemory = data->korKR.c_str();
	renderer.RenderFontInWorldCoordinate(m_textMemory.c_str(), (int)(m_textMemory.size()), m_fontSize, m_textPos, textColor);
}
