#include "stdafx.h"
#include "name_tag.h"
#include "per_renderer.h"
#include "per_database.h"

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

void NameTag::MatchWithData(PERVec2 pos, PERVec2 textPos)
{
	SetPosition(pos);
	m_textPosition = textPos;
}

void NameTag::Update(PERController& controller, PERAudio& audio, double dTime)
{
	if (!GetIsInUse()) return;
}

void NameTag::RenderOnScreen(PERRenderer& renderer, PERDatabase& database)
{
	if (!GetIsInUse()) return;

	PERVec2 pos = GetPosition();
	PERVec2 size = GetSize();
	// 바
	renderer.RenderShapeInScreenCoordinate(PERShapeType::RECTANGLE_WITH_LEFT_TOP_ANCHOR,
		pos, size, GetBackgroundColor(), false);
	// 이름 텍스트
	TranslateData* data = database.GetTranslateData(m_nameId.c_str());
	m_textMemory = data->korKR.c_str();
	renderer.RenderFontInScreenCoordinate(m_textMemory.c_str(), (int)m_textMemory.size(), m_fontSize, m_textPosition, m_textColor);
}

void NameTag::RenderInWorld(PERRenderer& renderer, PERDatabase& database)
{
	if (!GetIsInUse()) return;

	PERVec2 pos = GetPosition();
	PERVec2 size = GetSize();
	// 바
	renderer.RenderShapeInWorldCoordinate(PERShapeType::RECTANGLE_WITH_LEFT_TOP_ANCHOR,
		PERVec3(pos.x - size.x * 0.5, pos.y, 0.0), PERVec3(size.x, size.y, 0.0),
		GetBackgroundColor(), false);
	// 이름 텍스트
	TranslateData* data = database.GetTranslateData(m_nameId.c_str());
	m_textMemory = data->korKR.c_str();
	renderer.RenderFontInWorldCoordinate(m_textMemory.c_str(), (int)(m_textMemory.size()), m_fontSize,
		PERVec2(m_textPosition.x - size.x / 9.0 * (double)(m_textMemory.size()), m_textPosition.y), m_textColor);

}
