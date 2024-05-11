#include "stdafx.h"
#include "key_input_helper_graphics_component.h"
#include "black_board.h"
#include "key_input_helper.h"
#include "per_hud.h"
#include "key_setting.h"
#include "per_object.h"

void KeyInputHelperGraphicsComponent::Update(PERHud& hud, PERAudio& audio, double dTime)
{
	m_position = GetOwner()->GetPosition();
	m_size = GetOwner()->GetSize();

	UpdateHelper(hud);

	GraphicsComponent::Update(hud, audio, dTime);
}

void KeyInputHelperGraphicsComponent::Render(PERRenderer& renderer, double frameGap)
{
	GraphicsComponent::Render(renderer, frameGap);
}

void KeyInputHelperGraphicsComponent::SetData(PERComponent::GraphicsData data)
{
	GraphicsComponent::SetData(data);
}

void KeyInputHelperGraphicsComponent::Initialize()
{
	m_isShowingHelper = false;
	m_Helper = nullptr;

	GraphicsComponent::Initialize();
}

void KeyInputHelperGraphicsComponent::RemoveFloatingUi()
{
	if (m_Helper) m_Helper->SetLifeTime(-1.0);

	GraphicsComponent::RemoveFloatingUi();
}

void KeyInputHelperGraphicsComponent::UpdateHelper(PERHud& hud)
{
	PERVec3 playerPos = BlackBoard::GetPlayerPos();

	if (!m_isShowingHelper)
	{
		if (c_SHOWING_BODY_BAR_DISTANCE_2 < DistanceSquareAandBIgnoringZValue(playerPos, m_position)) return;
		if (ShowHelper(hud))
		{
			MatchHelperWithData();
			m_isShowingHelper = true;
		}
	}
	else
	{
		if (c_SHOWING_BODY_BAR_DISTANCE_2 < DistanceSquareAandBIgnoringZValue(playerPos, m_position))
		{
			HideHelper();
			m_isShowingHelper = false;
			return;
		}
		MatchHelperWithData();
	}
}

bool KeyInputHelperGraphicsComponent::ShowHelper(PERHud& hud)
{
	m_Helper = dynamic_cast<KeyInputHelper*>(hud.GetNewUiElementInWorld(PERUiElementType::KEY_INPUT_HELPER));
	if (!m_Helper) return false;

	m_Helper->Init(PERVec2(m_position.x , m_position.y + m_size.y * 0.5), PERVec2(0.5, 0.5), PERColor(200, 200, 200), PERColor(50, 50, 50), 
		KeySetting::Use, 0.5, PERColor(0, 0, 0), PERColor(250, 250, 250));
	return true;
}

void KeyInputHelperGraphicsComponent::MatchHelperWithData()
{
	PERVec2 pos = PERVec2(m_position.x, m_position.y + m_size.y * 1.5);
	m_Helper->MatchWithData(pos);
}

void KeyInputHelperGraphicsComponent::HideHelper()
{
	m_Helper->SetLifeTime(-1.0);
	m_Helper = nullptr;
}
