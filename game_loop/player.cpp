#include "stdafx.h"
#include "player.h"

Player::Player()
{
	SetPos(0, 0);
}

Player::~Player()
{
}

void Player::ProcessInputs(KeyInputs& inputs)
{
	int x, y;
	GetPos(&x, &y);

	if (inputs.left) x--;
	if (inputs.right) x++;
	if (inputs.up) y++;
	if (inputs.down) y--;

	MoveObjectToWorld(&x, &y);

	SetPos(x, y);
}


void Player::Update()
{
	m_exp++;

	int needExp = m_level * 10;
	if (needExp <= m_exp) {
		m_level++;
		m_exp = 0;
		m_showLevelUpPopupTimer = LEVEL_UP_POP_UP_SHOWING_TIME;
	}

	if (m_showLevelUpPopupTimer > 0) m_showLevelUpPopupTimer--;
}

void Player::Render()
{
	int x, y;
	GetPos(&x, &y);
	MoveCurser(x, y);
	std::cout << "P";
}

void Player::RenderLevel()
{
	MoveCurser(-10, -10);
	std::cout << "Player Level: " << m_level;
	RenderLevelUpPopup();
}

void Player::RenderLevelUpPopup()
{
	MoveCurser(-10, -11);
	if (m_showLevelUpPopupTimer > 0) std::cout << "LEVEL UP!!";
}
