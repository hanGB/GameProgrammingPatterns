#include "stdafx.h"
#include "visible_with_information_graphics_componenet.h"
#include "black_board.h"
#include "per_hud.h"
#include "per_object.h"

void VisibleWithInformationGraphicsComponent::Update(PERHud& hud, PERAudio& audio, double dTime)
{
	VisibleGraphicsComponent::Update(hud, audio, dTime);
	UpdateNameTag(hud);
}

void VisibleWithInformationGraphicsComponent::Render(PERRenderer& renderer, double frameGap)
{
	VisibleGraphicsComponent::Render(renderer, frameGap);
}

void VisibleWithInformationGraphicsComponent::SetData(PERComponent::GraphicsData data)
{
	VisibleGraphicsComponent::SetData(data);
}

void VisibleWithInformationGraphicsComponent::Initialize(PERComponent::GraphicsData data)
{
	m_isShowingNameTag = false;
	m_nameTag = nullptr;
	SetData(data);
}

void VisibleWithInformationGraphicsComponent::RemoveFloatingUi()
{
	VisibleGraphicsComponent::RemoveFloatingUi();
	if (m_nameTag)m_nameTag->SetLifeTime(-1.0);
}

void VisibleWithInformationGraphicsComponent::UpdateNameTag(PERHud& hud)
{
	if (!m_isShowingNameTag)
	{
		if (!BlackBoard::GetShowingName()) return;
		PERVec3 playerPos = BlackBoard::GetPlayerPos();
		if (c_SHOWING_NAME_TAG_DISTANCE_2 < DistanceSquareAandBIgnoringZValue(playerPos, m_position)) return;
		if (ShowNameTag(hud)) 
		{
			MatchNameTagWithData();
			m_isShowingNameTag = true;
		}
	}
	else
	{
		if (!BlackBoard::GetShowingName())
		{
			HideNameTag();
			m_isShowingNameTag = false;
			return;
		}
		PERVec3 playerPos = BlackBoard::GetPlayerPos();
		if (c_SHOWING_NAME_TAG_DISTANCE_2 < DistanceSquareAandBIgnoringZValue(playerPos, m_position))
		{
			HideNameTag();
			m_isShowingNameTag = false;
			return;
		}

		MatchNameTagWithData();
	}
}

bool VisibleWithInformationGraphicsComponent::ShowNameTag(PERHud& hud)
{
	m_nameTag = dynamic_cast<NameTag*>(hud.GetNewUiElementInWorld(PERUiElementType::NAME_TAG));
	if (!m_nameTag) return false;

	m_nameTag->Init(PERVec2(0.0, 0.0), PERVec2(1.4, 0.25), PERColor(200, 200, 200),
		GetOwner()->GetObjectState().GetNameId(), 0.35, PERVec2(0.0, 0.0), PERColor(50, 50, 50));
	return true;
}

void VisibleWithInformationGraphicsComponent::MatchNameTagWithData()
{
	PERVec2 pos = PERVec2(m_position.x - 0.2, m_position.y + 0.2);
	PERVec2 textPos = PERVec2(m_position.x - 0.2, m_position.y + 0.25);
	m_nameTag->MatchWithData(pos, textPos);
}

void VisibleWithInformationGraphicsComponent::HideNameTag()
{
	m_nameTag->SetLifeTime(-1.0);
	m_nameTag = nullptr;
}
