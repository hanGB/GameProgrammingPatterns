#include "stdafx.h"
#include "per_hud.h"
#include "per_renderer.h"
#include "progress_bar.h"

PERHud::PERHud()
{
	
}

PERHud::~PERHud()
{
	DeleteAllElementsOnScreen();
	DeleteAllElementsInWorld();
}

void PERHud::Update(PERAudio& audio, double dTime)
{
	DoGarbegeCollection();
	
	for (int i = 0; i < m_numElementInWorld; ++i) {
		m_uiElementsInWorld[i]->Update(audio, dTime);
	}
	for (int i = 0; i < m_numElementOnScreen; ++i) {
		m_uiElementsOnScreen[i]->Update(audio, dTime);
	}
}

void PERHud::Renderer(PERRenderer& renderer)
{
	for (int i = 0; i < m_numElementOnScreen; ++i) {
		m_uiElementsOnScreen[i]->RenderOnScreen(renderer);
	}
}

void PERHud::RendererInWorld(PERRenderer& renderer)
{
	for (int i = 0; i < m_numElementInWorld; ++i) {
		m_uiElementsInWorld[i]->RenderInWorld(renderer);
	}
}

void PERHud::Recive(PEREvent event, PERVec3 data)
{
	
}

UiElement* PERHud::GetNewUiElementInPool(PERUiElementType type)
{
	return m_uiElementPool->PopElement(type);
}

void PERHud::PushElementInWorld(UiElement* element)
{
	if (m_uiElementsInWorld.size() == m_numElementInWorld) {
		m_maxElementInWorld *= 2;
		m_uiElementsInWorld.reserve(m_maxElementInWorld);
		m_uiElementsInWorld.resize(m_maxElementInWorld);
	}
	m_uiElementsInWorld[m_numElementInWorld] = element;
	m_numElementInWorld++;
}

void PERHud::InitSettingForHud(UiElementPool* uiElementPool)
{
	PERLog::Logger().Info("HUD 생성");

	m_uiElementPool = uiElementPool;

	m_uiElementsOnScreen.reserve(m_maxElementOnScreen);
	m_uiElementsOnScreen.resize(m_maxElementOnScreen);

	m_uiElementsInWorld.reserve(m_maxElementOnScreen);
	m_uiElementsInWorld.resize(m_maxElementOnScreen);
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

void PERHud::DoGarbegeCollection()
{
	bool needToPop = true;
	int checkIndex = 0;

	// 더이상 꺼낼 게 없을 때까지 확인
	while (needToPop) {
		needToPop = false;
		for (int i = checkIndex; i < m_numElementInWorld; ++i) {
			if (!m_uiElementsInWorld[i]->GetIsLiving()) {
				needToPop = true;
				checkIndex = i;
				break;
			}
		}
		if (needToPop)PopElementInWorld(checkIndex);
	}
}

void PERHud::PopElementInWorld(int index)
{
	UiElement* element = m_uiElementsInWorld[index];
	m_uiElementsInWorld[index] = m_uiElementsInWorld[m_numElementInWorld - 1];

	m_uiElementPool->PushElement(element->GetType(), element);
	m_numElementInWorld--;
}

void PERHud::DeleteAllElementsOnScreen()
{
	for (int i = 0; i < m_numElementOnScreen; ++i) {
		m_uiElementPool->PushElement(m_uiElementsOnScreen[i]->GetType(), m_uiElementsOnScreen[i]);
	}
	m_uiElementsOnScreen.resize(0);
}

void PERHud::DeleteAllElementsInWorld()
{
	for (int i = 0; i < m_numElementInWorld; ++i) {
		m_uiElementPool->PushElement(m_uiElementsInWorld[i]->GetType(), m_uiElementsInWorld[i]);
	}
	m_uiElementsInWorld.resize(0);
}
