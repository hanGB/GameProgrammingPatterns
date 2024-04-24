#include "stdafx.h"
#include "monster_graphics_componenet.h"
#include "black_board.h"
#include "per_hud.h"
#include "per_object.h"

void MonsterGraphicsComponent::Update(PERObject& object, PERHud& hud, PERAudio& audio, double dTime)
{
	VisibleWithInformationGraphicsComponent::Update(object, hud, audio, dTime);
	UpdateBodyBar(object, hud);
}

void MonsterGraphicsComponent::Render(PERObject& object, PERRenderer& renderer, double frameGap)
{
	VisibleWithInformationGraphicsComponent::Render(object, renderer, frameGap);
}

void MonsterGraphicsComponent::SetData(PERComponent::GraphicsData data)
{
	VisibleWithInformationGraphicsComponent::SetData(data);
}

void MonsterGraphicsComponent::RemoveFloatingUi()
{
	VisibleWithInformationGraphicsComponent::RemoveFloatingUi();
	if (m_bodyBar)m_bodyBar->SetLifeTime(-1.0);
}

void MonsterGraphicsComponent::UpdateBodyBar(PERObject& object, PERHud& hud)
{
	PERVec3 playerPos = BlackBoard::GetPlayerPos();

	if (!m_isShowingBodyBar)
	{
		if (c_SHOWING_BODY_BAR_DISTANCE_2 < DistanceSquareAandBIgnoringZValue(playerPos, m_position)) return;
		if (ShowBodyBar(object, hud))
		{
			MatchBodyBarWithData(object);
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
		MatchBodyBarWithData(object);
	}
}

bool MonsterGraphicsComponent::ShowBodyBar(PERObject& object, PERHud& hud)
{
	m_bodyBar = dynamic_cast<ProgressBar*>(hud.GetNewUiElementInWorld(PERUiElementType::PROGRESS_BAR));
	if (!m_bodyBar) return false;

	m_bodyBar->Init(PERVec2(0.0, 0.0), PERVec2(1.0, 0.2), PERColor(200, 200, 200), PERColor(255, 0, 0), 100, 100);
	return true;
}

void MonsterGraphicsComponent::MatchBodyBarWithData(PERObject& object)
{
	PERVec2 pos = PERVec2(m_position.x, m_position.y + m_size.y * 1.5);
	m_bodyBar->MatchWithData(pos, object.GetObjectState().GetStat().body, object.GetObjectState().GetCurrentBody());
}

void MonsterGraphicsComponent::HideBodyBar()
{
	m_bodyBar->SetLifeTime(-1.0);
	m_bodyBar = nullptr;
}
