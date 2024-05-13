#include "stdafx.h"
#include "test_field_hud.h"

TestFieldHud::TestFieldHud()
{
	m_bodyBar = dynamic_cast<ProgressBar*>(m_uiElementPool->Create(PERUiElementType::PROGRESS_BAR));
	if (m_bodyBar)
	{
		m_bodyBar->Init(PERVec2(-0.95, 0.9), PERVec2(0.4, 0.1), PERColor(200, 200, 200), PERColor(255, 0, 0), 100, 100);
	}
	else
	{
		PERLog::Logger().ErrorWithFormat("Test field hud의 체력 바가 생성되지 않았습니다.");
	}

	m_mindBar = dynamic_cast<ProgressBar*>(m_uiElementPool->Create(PERUiElementType::PROGRESS_BAR));
	if (m_mindBar)
	{
		m_mindBar->Init(PERVec2(-0.95, 0.8), PERVec2(0.4, 0.1), PERColor(200, 200, 200), PERColor(0, 0, 255), 100, 100);
	}
	else
	{
		PERLog::Logger().ErrorWithFormat("Test field hud의 정신 바가 생성되지 않았습니다.");
	}
}

TestFieldHud::~TestFieldHud()
{

}

void TestFieldHud::Recive(PEREvent event, PERVec3 data)
{
	switch (event) {
	case PEREvent::UPDATE_BD:
		m_bodyBar->SetCurrent((int)data.x);
		if ((bool)data.y) m_bodyBar->UpateShowingValueImmediately();
		break;
	case PEREvent::UPDATE_MD:
		m_mindBar->SetCurrent((int)data.x);
		if ((bool)data.y) m_mindBar->UpateShowingValueImmediately();
		break;
	}
}

ProgressBar* TestFieldHud::GetBodyBar()
{
	return m_bodyBar;
}

ProgressBar* TestFieldHud::GetMindBar()
{
	return m_mindBar;
}
