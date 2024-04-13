#include "stdafx.h"
#include "test_field_hud.h"

TestFieldHud::TestFieldHud(UiElementPool* uiElementPool)
{
	InitSettingForHud(uiElementPool);

	ProgressBar* bodyBar = dynamic_cast<ProgressBar*>(GetNewUiElementInPool(PERUiElementType::PROGRESS_BAR));
	bodyBar->SetPosition(PERVec2(-0.95, 0.9)); bodyBar->SetSize(PERVec2(0.4, 0.1));
	bodyBar->SetColor(PERColor(200, 200, 200), PERColor(255, 0, 0));
	m_bodyBarIndex = PushElementOnScreen(bodyBar);

	ProgressBar* mindBar = dynamic_cast<ProgressBar*>(GetNewUiElementInPool(PERUiElementType::PROGRESS_BAR));
	mindBar->SetPosition(PERVec2(-0.95, 0.8)); mindBar->SetSize(PERVec2(0.4, 0.1));
	mindBar->SetColor(PERColor(200, 200, 200), PERColor(0, 0, 255));
	m_mindBarIndex = PushElementOnScreen(mindBar);
}

TestFieldHud::~TestFieldHud()
{

}

void TestFieldHud::Recive(PEREvent event, PERVec3 data)
{
	switch (event) {
	case PEREvent::UPDATE_BD:
		GetBodyBar()->SetCurrent((int)data.x);
		break;
	case PEREvent::UPDATE_MD:
		GetMindBar()->SetCurrent((int)data.x);
		break;
	}
}

ProgressBar* TestFieldHud::GetBodyBar()
{
	return dynamic_cast<ProgressBar*>(m_uiElementsOnScreen[m_bodyBarIndex]);
}

ProgressBar* TestFieldHud::GetMindBar()
{
	return dynamic_cast<ProgressBar*>(m_uiElementsOnScreen[m_mindBarIndex]);
}