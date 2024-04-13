#include "stdafx.h"
#include "per_hud.h"
#include "per_renderer.h"
#include "progress_bar.h"

PERHud::PERHud()
{
	
}

PERHud::~PERHud()
{
	for (int i = 0; i < m_numElementOnScreen; ++i) {
		m_uiElementPool->PushElement(m_uiElementsOnScreen[i]->GetType(), m_uiElementsOnScreen[i]);
	}

	m_uiElementsOnScreen.resize(0);
}

void PERHud::Update(PERAudio& audio, double dTime)
{
	for (int i = 0; i < m_numElementOnScreen; ++i) {
		m_uiElementsOnScreen[i]->Update(audio, dTime);
	}
}

void PERHud::Renderer(PERRenderer& renderer)
{
	for (int i = 0; i < m_numElementOnScreen; ++i) {
		m_uiElementsOnScreen[i]->RenderInScreen(renderer);
	}
}

void PERHud::Recive(PEREvent event, PERVec3 data)
{
	
}

void PERHud::InitSettingForHud(UiElementPool* uiElementPool)
{
	PERLog::Logger().Info("HUD »ý¼º");

	m_uiElementPool = uiElementPool;

	m_uiElementsOnScreen.reserve(m_maxElementOnScreen);
	m_uiElementsOnScreen.resize(m_maxElementOnScreen);
}

int PERHud::PushElementOnScreen(UiElement* element)
{
	if (m_uiElementsOnScreen.size() == m_numElementOnScreen) {
		m_maxElementOnScreen *= 2;
		m_uiElementsOnScreen.reserve(m_maxElementOnScreen);
		m_uiElementsOnScreen.resize(m_maxElementOnScreen);
	}
	m_uiElementsOnScreen[m_numElementOnScreen] = element;
	m_numElementOnScreen++;
	return m_numElementOnScreen - 1;
}
