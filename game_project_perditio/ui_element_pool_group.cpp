#include "stdafx.h"
#include "ui_element_pool_group.h"

UiElementPoolGroup::UiElementPoolGroup()
{
}

UiElementPoolGroup::~UiElementPoolGroup()
{
}

void UiElementPoolGroup::Update(PERController& controller, PERAudio& audio, double dTime)
{
	DoGarbegeCollection(dTime);

	m_progressBarPool.Update(controller, audio, dTime);
	m_nameTagPool.Update(controller, audio, dTime);
	m_keyInputHelperPool.Update(controller, audio, dTime);
	m_textViewerPool.Update(controller, audio, dTime);
	m_selectBoxPool.Update(controller, audio, dTime);
}

void UiElementPoolGroup::Renderer(PERRenderer& renderer, PERDatabase& database)
{
	m_progressBarPool.Renderer(renderer, database);
	m_nameTagPool.Renderer(renderer, database);
	m_keyInputHelperPool.Renderer(renderer, database);
	m_textViewerPool.Renderer(renderer, database);
	m_selectBoxPool.Renderer(renderer, database);
}

void UiElementPoolGroup::RendererInWorld(PERRenderer& renderer, PERDatabase& database)
{
	m_progressBarPool.RendererInWorld(renderer, database);
	m_nameTagPool.RendererInWorld(renderer, database);
	m_keyInputHelperPool.RendererInWorld(renderer, database);
	m_textViewerPool.RendererInWorld(renderer, database);
	m_selectBoxPool.RendererInWorld(renderer, database);
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
		case PERUiElementType::KEY_INPUT_HELPER: return m_keyInputHelperPool.CreateElementInWorld();
			break;
		case PERUiElementType::TEXT_VIEWER: return m_textViewerPool.CreateElementInWorld();
			break;
		case PERUiElementType::SELECT_BOX: return m_selectBoxPool.CreateElementInWorld();
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
		case PERUiElementType::KEY_INPUT_HELPER: return m_keyInputHelperPool.CreateElementOnScreen();
			break;
		case PERUiElementType::TEXT_VIEWER: return m_textViewerPool.CreateElementOnScreen();
			break;
		case PERUiElementType::SELECT_BOX: return m_selectBoxPool.CreateElementOnScreen();
			break;
		}
	}

	return nullptr;
}

void UiElementPoolGroup::DoGarbegeCollection(double dTime)
{
	m_progressBarPool.DoGarbegeCollection(dTime);
	m_nameTagPool.DoGarbegeCollection(dTime);
	m_keyInputHelperPool.DoGarbegeCollection(dTime);
	m_textViewerPool.DoGarbegeCollection(dTime);
	m_selectBoxPool.DoGarbegeCollection(dTime);
}
