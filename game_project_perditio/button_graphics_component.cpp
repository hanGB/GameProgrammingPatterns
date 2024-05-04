#include "stdafx.h"
#include "button_graphics_component.h"
#include "black_board.h"
#include "key_input_helper.h"
#include "per_hud.h"
#include "key_setting.h"

void ButtonGraphicsComponent::Update(PERObject& object, PERHud& hud, PERAudio& audio, double dTime)
{
	VisibleWithInformationGraphicsComponent::Update(object, hud, audio, dTime);
	UpdateHelper(object, hud);
}

void ButtonGraphicsComponent::Render(PERObject& object, PERRenderer& renderer, double frameGap)
{
	VisibleWithInformationGraphicsComponent::Render(object, renderer, frameGap);
}

void ButtonGraphicsComponent::SetData(PERComponent::GraphicsData data)
{
	VisibleWithInformationGraphicsComponent::SetData(data);
}

void ButtonGraphicsComponent::Initialize(PERComponent::GraphicsData data)
{
	m_isShowingHelper = false;
	m_Helper = nullptr;
	SetData(data);
}

void ButtonGraphicsComponent::RemoveFloatingUi()
{
	if (m_Helper) m_Helper->SetLifeTime(-1.0);
}

void ButtonGraphicsComponent::UpdateHelper(PERObject& object, PERHud& hud)
{
	PERVec3 playerPos = BlackBoard::GetPlayerPos();

	if (!m_isShowingHelper)
	{
		if (c_SHOWING_BODY_BAR_DISTANCE_2 < DistanceSquareAandBIgnoringZValue(playerPos, m_position)) return;
		if (ShowHelper(object, hud))
		{
			MatchHelperWithData(object);
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
		MatchHelperWithData(object);
	}
}

bool ButtonGraphicsComponent::ShowHelper(PERObject& object, PERHud& hud)
{
	m_Helper = dynamic_cast<KeyInputHelper*>(hud.GetNewUiElementInWorld(PERUiElementType::KEY_INPUT_HELPER));
	if (!m_Helper) return false;

	m_Helper->Init(PERVec2(m_position.x , m_position.y + m_size.y * 0.5), PERVec2(0.5, 0.5), PERColor(200, 200, 200), PERColor(50, 50, 50), 
		KeySetting::Use, 0.5, PERColor(0, 0, 0), PERColor(250, 250, 250));
	return true;
}

void ButtonGraphicsComponent::MatchHelperWithData(PERObject& object)
{
	// button�� �����Ǿ� �ֱ� ������ ��ġ�� ������ �ʿ� ����
	// PERVec2 pos = PERVec2(m_position.x, m_position.y + m_size.y * 1.5);
	// m_Helper->MatchWithData(pos);
}

void ButtonGraphicsComponent::HideHelper()
{
	m_Helper->SetLifeTime(-1.0);
	m_Helper = nullptr;
}