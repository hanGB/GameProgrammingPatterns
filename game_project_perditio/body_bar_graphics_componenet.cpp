#include "stdafx.h"
#include "body_bar_graphics_componenet.h"
#include "black_board.h"
#include "per_hud.h"
#include "per_object.h"

void BodyBarGraphicsComponent::Update(PERHud& hud, PERAudio& audio, double dTime)
{
	m_position = GetOwner()->GetPosition();
	m_size = GetOwner()->GetSize();

	UpdateBodyBar(hud);

	GraphicsComponent::Update(hud, audio, dTime);
}

void BodyBarGraphicsComponent::Render(PERRenderer& renderer, double frameGap)
{
	GraphicsComponent::Render(renderer, frameGap);
}

void BodyBarGraphicsComponent::SetData(PERComponent::GraphicsData data)
{
	GraphicsComponent::SetData(data);
}

void BodyBarGraphicsComponent::Initialize()
{
	m_isShowingBodyBar = false;
	m_bodyBar = nullptr;

	GraphicsComponent::Initialize();
}

void BodyBarGraphicsComponent::RemoveFloatingUi()
{
	if (m_bodyBar)m_bodyBar->SetLifeTime(-1.0);

	GraphicsComponent::RemoveFloatingUi();
}

void BodyBarGraphicsComponent::UpdateBodyBar(PERHud& hud)
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

bool BodyBarGraphicsComponent::ShowBodyBar(PERHud& hud)
{
	m_bodyBar = dynamic_cast<ProgressBar*>(hud.GetNewUiElementInWorld(PERUiElementType::PROGRESS_BAR));
	if (!m_bodyBar) return false;

	m_bodyBar->Init(PERVec2(0.0, 0.0), PERVec2(1.0, 0.2), PERColor(200, 200, 200), PERColor(255, 0, 0), 100, 100);
	return true;
}

void BodyBarGraphicsComponent::MatchBodyBarWithData()
{
	PERVec2 pos = PERVec2(m_position.x, m_position.y + m_size.y * 1.5);
	m_bodyBar->MatchWithData(pos, GetOwner()->GetObjectState().GetStat().body, GetOwner()->GetObjectState().GetCurrentBody());
}

void BodyBarGraphicsComponent::HideBodyBar()
{
	m_bodyBar->SetLifeTime(-1.0);
	m_bodyBar = nullptr;
}
