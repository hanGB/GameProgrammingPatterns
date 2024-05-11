#include "stdafx.h"
#include "hidden_graphics_component.h"

void HiddenGraphicsComponent::Update(PERHud& hud, PERAudio& audio, double dTime)
{
	GraphicsComponent::Update(hud, audio, dTime);
}

void HiddenGraphicsComponent::Render(PERRenderer& renderer, double frameGap)
{
	GraphicsComponent::Render(renderer, frameGap);
}

void HiddenGraphicsComponent::SetData(PERComponent::GraphicsData data)
{
	GraphicsComponent::SetData(data);
}

void HiddenGraphicsComponent::Initialize()
{
	GraphicsComponent::Initialize();
}

void HiddenGraphicsComponent::RemoveFloatingUi()
{
	GraphicsComponent::RemoveFloatingUi();
}
