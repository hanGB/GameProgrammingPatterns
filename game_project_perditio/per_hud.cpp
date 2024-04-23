#include "stdafx.h"
#include "per_hud.h"
#include "per_renderer.h"
#include "progress_bar.h"

PERHud::PERHud()
{
	m_uiElementPool = new UiElementPoolGroup();
}

PERHud::~PERHud()
{
	delete m_uiElementPool;
}

void PERHud::Update(PERAudio& audio, double dTime)
{
	m_uiElementPool->Update(audio, dTime);
}

void PERHud::Renderer(PERRenderer& renderer, PERDatabase& database)
{
	m_uiElementPool->Renderer(renderer, database);
}

void PERHud::RendererInWorld(PERRenderer& renderer, PERDatabase& database)
{
	m_uiElementPool->RendererInWorld(renderer, database);
}

void PERHud::Recive(PEREvent event, PERVec3 data)
{
	
}

UiElement* PERHud::GetNewUiElementInWorld(PERUiElementType type)
{
	return m_uiElementPool->Create(type, true);
}
