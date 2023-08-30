#include "stdafx.h"
#include "per_hud.h"
#include "per_renderer.h"

PERHud::PERHud()
{
	PERLog::Logger().Info("HUD »ý¼º");

	m_bodyBar = new ProgressBar(PERVec2(-0.95, 0.9), 100, 100);
	m_bodyBar->SetColor(PERColor(255, 255, 255), PERColor(255, 0, 0));

	m_mindBar = new ProgressBar(PERVec2(-0.95, 0.8), 100, 100);
	m_mindBar->SetColor(PERColor(255, 255, 255), PERColor(0, 0, 255));
}

PERHud::~PERHud()
{
	delete m_bodyBar;
	delete m_mindBar;
}

void PERHud::Update(PERAudio& audio, double dTime)
{
	m_bodyBar->Update(audio, dTime);
	m_mindBar->Update(audio, dTime);
}

void PERHud::Renderer(PERRenderer& renderer)
{
	m_bodyBar->Render(renderer);
	m_mindBar->Render(renderer);
}

void PERHud::Recive(PEREvent event, PERVec3 data)
{
	switch (event) {
	case PEREvent::UPDATE_HP:
		m_bodyBar->SetCurrent((int)data.x);
		break;
	case PEREvent::UPDATE_MP: 
		m_mindBar->SetCurrent((int)data.x);
		break;
	}
}

ProgressBar* PERHud::GetBodyBar()
{
	return m_bodyBar;
}

ProgressBar* PERHud::GetMindBar()
{
	return m_mindBar;
}
