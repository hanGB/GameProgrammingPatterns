#include "stdafx.h"
#include "visible_graphics_component.h"
#include "per_object.h"
#include "per_renderer.h"

void VisibleGraphicsComponent::Update(PERObject& object, double dTime)
{
}

void VisibleGraphicsComponent::Render(PERObject& object, PERRenderer& renderer)
{
	PERVec3 pos = object.GetPosition();
	PERVec2 size = object.GetSize();

	renderer.RenderShape(m_shapeType, pos, size, m_color);
}

void VisibleGraphicsComponent::SetShapeType(PERShapeType type)
{
	m_shapeType = type;
}

void VisibleGraphicsComponent::SetColor(PERColor color)
{
	m_color = color;
}
