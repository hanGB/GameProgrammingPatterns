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

void NameTag::MatchWithData(std::string nameId, PERVec2 textPos)
{
	m_nameId = nameId;
	m_textPosition = textPos;
}

void NameTag::Update(PERAudio& audio, double dTime)
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
	USES_CONVERSION;
	std::wstring name = A2W(data->korKR.c_str());
	renderer.RenderFontInScreenCoordinate(name.c_str(), name.size(), m_fontSize, m_textPosition, m_textColor);
}

void NameTag::RenderInWorld(PERRenderer& renderer, PERDatabase& database)
{
	if (!GetIsInUse()) return;
	
	// 이름 아이디로 번역 데이터베이스에서 실제 이름 가져오기
	TranslateData* data = database.GetTranslateData(m_nameId.c_str());
	USES_CONVERSION;
	std::wstring name = A2W(data->korKR.c_str());
	PERVec2 pos = GetPosition();
	PERVec2 size = GetSize();

	// 배경 색상은 글자 색의 정반대 색으로
	PERColor backgroundColor = PERColor(255 - m_textColor.r, 255 - m_textColor.g, 255 - m_textColor.b);

	// 바
	renderer.RenderShapeInWorldCoordinate(PERShapeType::RECTANGLE_WITH_LEFT_TOP_ANCHOR,
		PERVec3(pos.x - size.x * 0.5, pos.y, 0.0), PERVec3(size.x, size.y, 0.0),
		backgroundColor, false);
	// 이름 텍스트
	renderer.RenderFontInWorldCoordinate(name.c_str(), (int)(name.size()), m_fontSize, PERVec2(m_textPosition.x - size.x / 9.0 * (double)(name.size()), m_textPosition.y), m_textColor);

}
