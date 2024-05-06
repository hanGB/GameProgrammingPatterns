#include "stdafx.h"
#include "monster_graphics_componenet.h"
#include "black_board.h"
#include "per_hud.h"
#include "per_object.h"

void MonsterGraphicsComponent::Update(PERHud& hud, PERAudio& audio, double dTime)
{
	VisibleWithInformationGraphicsComponent::Update(hud, audio, dTime);
	UpdateBodyBar(hud);
}

void MonsterGraphicsComponent::Render(PERRenderer& renderer, double frameGap)
{
	VisibleWithInformationGraphicsComponent::Render(renderer, frameGap);
}

void MonsterGraphicsComponent::SetData(PERComponent::GraphicsData data)
{
	VisibleWithInformationGraphicsComponent::SetData(data);
}

void MonsterGraphicsComponent::Initialize(PERComponent::GraphicsData data)
{
	m_isShowingBodyBar = false;
	m_bodyBar = nullptr;
	SetData(data);
}

void MonsterGraphicsComponent::RemoveFloatingUi()
{
	VisibleWithInformationGraphicsComponent::RemoveFloatingUi();
	if (m_bodyBar)m_bodyBar->SetLifeTime(-1.0);
}

void MonsterGraphicsComponent::UpdateBodyBar(PERHud& hud)
{
	PERVec3 playerPos = BlackBoard::GetPlayerPos();

	if (!m_isShowingBodyBar)
	{
		if (c_SHOWING_BODY_BAR_DISTANCE_2 < DistanceSquareAandBIgnoringZValue(playerPos, m_position)) return;
		if (ShowBodyBar(hud))
		{
			MatchBodyBarWithData();
			m_isShowingBodyBar = true;
		}
	}
	else
	{
		if (c_SHOWING_BODY_BAR_DISTANCE_2 < DistanceSquareAandBIgnoringZValue(playerPos, m_position))
		{
			HideBodyBar();
			m_isShowingBodyBar = false;
			return;
		}
		MatchBodyBarWithData();
	}
}

bool MonsterGraphicsComponent::ShowBodyBar(PERHud& hud)
{
	m_bodyBar = dynamic_cast<ProgressBar*>(hud.GetNewUiElementInWorld(PERUiElementType::PROGRESS_BAR));
	if (!m_bodyBar) return false;

	m_bodyBar->Init(PERVec2(0.0, 0.0), PERVec2(1.0, 0.2), PERColor(200, 200, 200), PERColor(255, 0, 0), 100, 100);
	return true;
}

void MonsterGraphicsComponent::MatchBodyBarWithData()
{
	PERVec2 pos = PERVec2(m_position.x, m_position.y + m_size.y * 1.5);
	m_bodyBar->MatchWithData(pos, GetOwner()->GetObjectState().GetStat().body, GetOwner()->GetObjectState().GetCurrentBody());
}

void MonsterGraphicsComponent::HideBodyBar()
{
	m_bodyBar->SetLifeTime(-1.0);
	m_bodyBar = nullptr;
}
