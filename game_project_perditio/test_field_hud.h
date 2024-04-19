#pragma once
#include "per_hud.h"
#include "ui_element_pool.h"
#include "object_state.h"

class TestFieldHud : public PERHud {
public:
	TestFieldHud(UiElementPool* uiElementPool);
	~TestFieldHud();

	virtual void Recive(PEREvent event, PERVec3 data);

	ProgressBar* GetBodyBar();
	ProgressBar* GetMindBar();

private:
	ProgressBar* m_bodyBar, * m_mindBar;
};