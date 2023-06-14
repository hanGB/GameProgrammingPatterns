#include "stdafx.h"
#include "player_graphics_component.h"
#include "per_object.h"
#include "per_renderer.h"

void PlayerGraphicsComponent::Update(PERObject& object, double dTime)
{
}

void PlayerGraphicsComponent::Render(PERObject& object, PERRenderer& renderer)
{
	PERVec3 pos = object.GetPosition();
	PERVec2 size = object.GetSize();

	renderer.RenderShape(m_shapeType, pos, size, m_color);
}
