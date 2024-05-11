#include "stdafx.h"
#include "name_tag_graphics_component.h"
#include "black_board.h"
#include "per_hud.h"
#include "per_object.h"

void NameTagGraphicsComponent::Update(PERHud& hud, PERAudio& audio, double dTime)
{
	m_position = GetOwner()->GetPosition();

	UpdateNameTag(hud);

	GraphicsComponent::Update(hud, audio, dTime);
}

void NameTagGraphicsComponent::Render(PERRenderer& renderer, double frameGap)
{
	GraphicsComponent::Render(renderer, frameGap);
}

void NameTagGraphicsComponent::SetData(PERComponent::GraphicsData data)
{
	GraphicsComponent::SetData(data);
}

void NameTagGraphicsComponent::Initialize()
{
	m_isShowingNameTag = false;
	m_nameTag = nullptr;

	GraphicsComponent::Initialize();
}

void NameTagGraphicsComponent::RemoveFloatingUi()
{
	if (m_nameTag) m_nameTag->SetLifeTime(-1.0);

	GraphicsComponent::RemoveFloatingUi();
}

void NameTagGraphicsComponent::UpdateNameTag(PERHud& hud)
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

bool NameTagGraphicsComponent::ShowNameTag(PERHud& hud)
{
	m_nameTag = dynamic_cast<NameTag*>(hud.GetNewUiElementInWorld(PERUiElementType::NAME_TAG));
	if (!m_nameTag) return false;

	m_nameTag->Init(PERVec2(0.0, 0.0), PERVec2(1.4, 0.25), PERColor(200, 200, 200),
		GetOwner()->GetObjectState().GetNameId(), 0.35, PERVec2(0.0, 0.0), PERColor(50, 50, 50));
	return true;
}

void NameTagGraphicsComponent::MatchNameTagWithData()
{
	PERVec2 pos = PERVec2(m_position.x - 0.2, m_position.y + 0.2);
	PERVec2 textPos = PERVec2(m_position.x - 0.2, m_position.y + 0.25);
	m_nameTag->MatchWithData(pos, textPos);
}

void NameTagGraphicsComponent::HideNameTag()
{
	m_nameTag->SetLifeTime(-1.0);
	m_nameTag = nullptr;
}
