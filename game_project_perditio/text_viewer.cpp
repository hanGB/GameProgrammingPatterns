#include "stdafx.h"
#include "text_viewer.h"
#include "per_renderer.h"
#include "per_database.h"

TextViewer::TextViewer()
{
}

TextViewer::~TextViewer()
{
}

void TextViewer::Init(PERVec2 pos, double fontSize, PERColor color, const char* textId)
{
	UiElement::Init(pos, PERVec2(fontSize, 0.0), color);

	m_textId = textId;
}

void TextViewer::MatchWithData(PERVec2 pos)
{
	SetPosition(pos);
}

void TextViewer::Update(PERController& controller, PERAudio& audio, double dTime)
{
	if (!GetIsInUse()) return;
}

void TextViewer::RenderOnScreen(PERRenderer& renderer, PERDatabase& database)
{
	if (!GetIsInUse()) return;

	TranslateData* data = database.GetTranslateData(m_textId.c_str());
	m_textMemory = data->korKR.c_str();

	renderer.RenderFontInScreenCoordinate(m_textMemory.c_str(), (int)m_textMemory.size(), GetSize().x, GetPosition(), GetBackgroundColor());
}

void TextViewer::RenderInWorld(PERRenderer& renderer, PERDatabase& database)
{
	if (!GetIsInUse()) return;

	TranslateData* data = database.GetTranslateData(m_textId.c_str());
	m_textMemory = data->korKR.c_str();

	renderer.RenderFontInWorldCoordinate(m_textMemory.c_str(), (int)m_textMemory.size(), GetSize().x, GetPosition(), GetBackgroundColor());

}
