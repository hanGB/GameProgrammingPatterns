#include "stdafx.h"
#include "text_graphics_component.h"
#include "per_object.h"
#include "per_renderer.h"
#

void TextGraphicsComponent::Update(PERHud& hud, PERAudio& audio, double dTime)
{
	m_position = GetOwner()->GetPosition();
	m_size = GetOwner()->GetSize().x;
	SetPositionZValue(m_position.z);

	GraphicsComponent::Update(hud, audio, dTime);
}

void TextGraphicsComponent::Render(PERRenderer& renderer, double frameGap)
{
	renderer.RenderFontInWorldCoordinate(m_text.c_str(), (int)m_text.size(), m_size, PERVec2(m_position.x, m_position.y), m_color);

	GraphicsComponent::Render(renderer, frameGap);
}

void TextGraphicsComponent::SetData(PERComponent::GraphicsData data)
{
	GraphicsComponent::SetData(data);
}

void TextGraphicsComponent::Initialize()
{
	m_text = L"test";
	m_color = PERColor(0, 0, 0);

	GraphicsComponent::Initialize();
}

void TextGraphicsComponent::RemoveFloatingUi()
{
	GraphicsComponent::RemoveFloatingUi();
}

void TextGraphicsComponent::SetText(const wchar_t* text, PERColor color)
{
	m_text = text;
	m_color = color;
}
