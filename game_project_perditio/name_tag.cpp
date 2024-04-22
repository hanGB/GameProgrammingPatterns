#include "stdafx.h"
#include "name_tag.h"
#include "per_renderer.h"

NameTag::NameTag()
{
}

NameTag::~NameTag()
{
}

void NameTag::Init(PERVec2 pos, PERVec2 size, PERColor background, std::string nameId, double fontSize, PERVec2 textPos, PERColor textColor)
{
	UiElement::Init(pos, size, background);

	m_nameId = nameId;
	m_fontSize = fontSize;
	m_textPosition = textPos;
	m_textColor = textColor;
}

void NameTag::MatchWithData(std::string nameId, PERVec2 textPos)
{
	m_nameId = nameId;
	m_textPosition = textPos;
}

void NameTag::Update(PERAudio& audio, double dTime)
{
	if (!GetIsInUse()) return;
}

void NameTag::RenderOnScreen(PERRenderer& renderer)
{
	if (!GetIsInUse()) return;

	PERVec2 pos = GetPosition();
	PERVec2 size = GetSize();
	// 바
	renderer.RenderShapeInScreenCoordinate(PERShapeType::RECTANGLE_WITH_LEFT_TOP_ANCHOR,
		pos, size, GetBackgroundColor(), GetBorder(), GetBorderWidth(), GetBorderColor());
	// 이름 텍스트
	renderer.RenderFontInScreenCoordinate(L"name", sizeof(L"name") / sizeof(wchar_t), m_fontSize, m_textPosition, m_textColor);
}

void NameTag::RenderInWorld(PERRenderer& renderer)
{
	if (!GetIsInUse()) return;

	PERVec2 pos = GetPosition();
	PERVec2 size = GetSize();
	// 바
	renderer.RenderShapeInWorldCoordinate(PERShapeType::RECTANGLE_WITH_LEFT_TOP_ANCHOR,
		PERVec3(pos.x - size.x / 2.0, pos.y, 0.0), PERVec3(size.x, size.y, 0.0),
		GetBackgroundColor(), GetBorder(), GetBorderWidth(), GetBorderColor());
	// 이름 텍스트
	renderer.RenderFontInWorldCoordinate(L"name", sizeof(L"name") / sizeof(wchar_t), m_fontSize, m_textPosition, m_textColor);
}
