#include "stdafx.h"
#include "per_hud.h"
#include "per_renderer.h"

PERHud::PERHud()
{
	PERLog::Logger().Info("HUD »ı¼º");

	m_hpBar = new ProgressBar(PERVec2(-0.95, 0.9), 100, 100);
	m_hpBar->SetColor(PERColor(255, 255, 255), PERColor(255, 0, 0));

	m_mpBar = new ProgressBar(PERVec2(-0.95, 0.8), 100, 100);
	m_mpBar->SetColor(PERColor(255, 255, 255), PERColor(0, 0, 255));
}

PERHud::~PERHud()
{
	delete m_hpBar;
	delete m_mpBar;
}

void PERHud::Update(PERAudio* audio, double dTime)
{
	m_hpBar->Update(audio, dTime);
	m_mpBar->Update(audio, dTime);
}

void PERHud::Renderer(PERRenderer& renderer)
{
	m_hpBar->Render(renderer);
	m_mpBar->Render(renderer);
}

void PERHud::Recive(PEREvent event, PERVec3 data)
{
	switch (event) {
	case PEREvent::EVENT_UPDATE_HP:
		m_hpBar->SetCurrent((int)data.x);
		break;
	case PEREvent::EVENT_UPDATE_MP:
		m_mpBar->SetCurrent((int)data.x);
		break;
	}
}
