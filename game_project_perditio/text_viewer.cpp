#include "stdafx.h"
#include "text_viewer.h"
#include "per_renderer.h"

TextViewer::TextViewer()
{
}

TextViewer::~TextViewer()
{
}

void TextViewer::Init(PERVec2 pos, double fontSize, PERColor color, const wchar_t* text)
{
	UiElement::Init(pos, PERVec2(fontSize, 0.0), color);

	m_text = text;
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

	renderer.RenderFontInScreenCoordinate(m_text.c_str(), (int)m_text.size(), GetSize().x, GetPosition(), GetBackgroundColor());
}

void TextViewer::RenderInWorld(PERRenderer& renderer, PERDatabase& database)
{
	if (!GetIsInUse()) return;

	renderer.RenderFontInWorldCoordinate(m_text.c_str(), (int)m_text.size(), GetSize().x, GetPosition(), GetBackgroundColor());

}
