#include "stdafx.h"
#include "ui_element_pool_group.h"

UiElementPoolGroup::UiElementPoolGroup()
{
}

UiElementPoolGroup::~UiElementPoolGroup()
{
}

void UiElementPoolGroup::Update(PERAudio& audio, double dTime)
{
	DoGarbegeCollection(dTime);

	m_progressBarPool.Update(audio, dTime);
	m_nameTagPool.Update(audio, dTime);
}

void UiElementPoolGroup::Renderer(PERRenderer& renderer, PERDatabase& database)
{
	m_progressBarPool.Renderer(renderer, database);
	m_nameTagPool.Renderer(renderer, database);
}

void UiElementPoolGroup::RendererInWorld(PERRenderer& renderer, PERDatabase& database)
{
	m_progressBarPool.RendererInWorld(renderer, database);
	m_nameTagPool.RendererInWorld(renderer, database);
}

UiElement* UiElementPoolGroup::Create(PERUiElementType type, bool inWorld)
{
	if (inWorld)
	{
		switch (type) {
		case PERUiElementType::PROGRESS_BAR: return m_progressBarPool.CreateElementInWorld();
			break;
		case PERUiElementType::NAME_TAG: return m_nameTagPool.CreateElementInWorld();
			break;
		}
	}
	else
	{
		switch (type) {
		case PERUiElementType::PROGRESS_BAR: return m_progressBarPool.CreateElementOnScreen();
			break;
		case PERUiElementType::NAME_TAG: return m_nameTagPool.CreateElementOnScreen();
			break;
		}
	}

	return nullptr;
}

void UiElementPoolGroup::DoGarbegeCollection(double dTime)
{
	m_progressBarPool.DoGarbegeCollection(dTime);
	m_nameTagPool.DoGarbegeCollection(dTime);
}
