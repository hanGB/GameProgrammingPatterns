#include "stdafx.h"
#include "per_hud.h"
#include "per_renderer.h"
#include "progress_bar.h"

PERHud::PERHud()
{
	PERLog::Logger().Info("HUD »ý¼º");

	m_uiElements.reserve(m_maxElements);
	m_uiElements.resize(m_maxElements);

	ProgressBar* bodyBar = new ProgressBar(PERVec2(-0.95, 0.9), 100, 100);
	bodyBar->SetColor(PERColor(255, 255, 255), PERColor(255, 0, 0));
	m_bodyBarIndex = PushElement(bodyBar);

	ProgressBar* mindBar = new ProgressBar(PERVec2(-0.95, 0.8), 100, 100);
	mindBar->SetColor(PERColor(255, 255, 255), PERColor(0, 0, 255));
	m_mindBarIndex = PushElement(mindBar);
}

PERHud::~PERHud()
{
	int i = 0;
	for (auto& element : m_uiElements) {
		if (i == m_numElement) break;
		delete element;
	}
}

void PERHud::Update(PERAudio& audio, double dTime)
{
	int i = 0;
	for (auto& element : m_uiElements) {
		if (i == m_numElement) break;
		element->Update(audio, dTime);
	}
}

void PERHud::Renderer(PERRenderer& renderer)
{
	int i = 0;
	for (auto& element : m_uiElements) {
		if (i == m_numElement) break;
		element->RenderInScreen(renderer);
	}
}

void PERHud::Recive(PEREvent event, PERVec3 data)
{
	switch (event) {
	case PEREvent::UPDATE_HP:
		dynamic_cast<ProgressBar*>(m_uiElements[m_bodyBarIndex])->SetCurrent((int)data.x);
		break;
	case PEREvent::UPDATE_MP: 
		dynamic_cast<ProgressBar*>(m_uiElements[m_mindBarIndex])->SetCurrent((int)data.x);
		break;
	}
}

ProgressBar* PERHud::GetBodyBar()
{
	return dynamic_cast<ProgressBar*>(m_uiElements[m_bodyBarIndex]);
}

ProgressBar* PERHud::GetMindBar()
{
	return dynamic_cast<ProgressBar*>(m_uiElements[m_mindBarIndex]);
}

int PERHud::PushElement(UiElement* element)
{
	if (m_numElement == m_maxElements) {
		m_maxElements *= 2;
		m_uiElements.reserve(m_maxElements);
		m_uiElements.resize(m_maxElements);
	}
	m_uiElements.push_back(element);
	m_numElement++;
	return m_numElement - 1;
}
