#include "stdafx.h"
#include "hidden_debug_graphics_component.h"
#include "per_object.h"

void HiddenDebugGraphicsComponent::Update(PERHud& hud, PERAudio& audio, double dTime)
{
	m_position = GetOwner()->GetPosition();
	m_size = GetOwner()->GetSize();

	SetPositionZValue(m_position.z);

	GraphicsComponent::Update(hud, audio, dTime);
}

void HiddenDebugGraphicsComponent::Render(PERRenderer& renderer, double frameGap)
{
	if (m_isDebugOn)
		renderer.RenderShapeInWorldCoordinate(m_shapeType, m_position, m_size, m_color, m_border, m_borderWidth, m_borderColor);

	GraphicsComponent::Render(renderer, frameGap);
}

void HiddenDebugGraphicsComponent::SetData(PERComponent::GraphicsData data)
{
	GraphicsComponent::SetData(data);
}

void HiddenDebugGraphicsComponent::Initialize()
{
	m_isDebugOn = true;

	GraphicsComponent::Initialize();
}

void HiddenDebugGraphicsComponent::SetDebug(bool debug)
{
	m_isDebugOn = debug;
}
