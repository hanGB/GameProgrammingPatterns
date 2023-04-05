#pragma once
#include "eq_renderer.h"

enum class EqMenuId {
	EQ_MENU_ID_MAIN,
	EQ_MENU_ID_SKILL,
	EQ_MENU_ID_ITEM,
	EQ_MENU_ID_ACT
};

enum class EqActId {
	EQ_ACT_ID_MAIN,
	EQ_ACT_ID_CHANGE,
	EQ_ACT_ID_WAIT,
	EQ_ACT_ID_RUNAWAY,
	EQ_LAST_ACT
};

class EqUserInterface
{
public:
	EqUserInterface();
	~EqUserInterface();

	void Update(double elapsedTimeInSec);
	void Render(EqRenderer& renderer);

private:
	void UpdateMenuWithControl();
	void UseSelectedAct();

	void RenderMainMenu(EqRenderer& renderer);
	void RenderSubMenu(EqRenderer& renderer);
	void RenderTurns(EqRenderer& renderer);

	EqMenuId m_menuId = EqMenuId::EQ_MENU_ID_MAIN;
	EqActId m_actId = EqActId::EQ_ACT_ID_MAIN;
	int m_selectedMenuObjectInSkill = 0;
	int m_selectedMenuObjectInItem = 0;
	int m_selectedMenuObjectInAct = 0;

	int m_turns = 5;
};