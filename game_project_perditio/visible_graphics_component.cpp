#include "stdafx.h"
#include "visible_graphics_component.h"
#include "per_object.h"
#include "per_renderer.h"

void VisibleGraphicsComponent::Update(PERHud& hud, PERAudio& audio, double dTime)
{
	m_position = GetOwner()->GetPosition();
	m_size = GetOwner()->GetSize();
	m_currentVelocity = GetOwner()->GetVelocity();
	
	SetPositionZValue(m_position.z);

	GraphicsComponent::Update(hud, audio, dTime);
}

void VisibleGraphicsComponent::Render(PERRenderer& renderer, double frameGap)
{	
	PERVec3 renderPos = m_position;
	PERVec3 gap = m_currentVelocity * frameGap * ((double)PER_MICROSEC_PER_UPDATE / 1'000'000.0);
	renderPos = PERVec3(renderPos.x + gap.x, renderPos.y + gap.y, renderPos.z + gap.z);
	renderer.RenderShapeInWorldCoordinate(m_shapeType, renderPos, m_size, m_color, m_border, m_borderWidth, m_borderColor);

	GraphicsComponent::Render(renderer, frameGap);
}

void VisibleGraphicsComponent::SetData(PERComponent::GraphicsData data)
{
	m_shapeType = data.shape;
	m_color = data.color;

	m_border = data.border;
	m_borderWidth = data.borderWidth;
	m_borderColor = data.borderColor;

	GraphicsComponent::SetData(data);
}

void VisibleGraphicsComponent::Initialize()
{
	GraphicsComponent::Initialize();
}

void VisibleGraphicsComponent::RemoveFloatingUi()
{
	GraphicsComponent::RemoveFloatingUi();
}
