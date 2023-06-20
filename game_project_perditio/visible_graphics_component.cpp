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
	PERVec3 size = object.GetSize();

	renderer.RenderShape(m_shapeType, pos, size, m_color);
}

void VisibleGraphicsComponent::SetData(PERComponent::GraphicsData data)
{
	m_shapeType = data.shape;
	m_color = data.color;
}

