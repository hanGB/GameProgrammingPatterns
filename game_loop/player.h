#pragma once
#include "stdafx.h"
#include "snake.h"
#include "tail.h"
#include <array>

class Player : public Snake {
public:
	Player() 
	{ 
		SetType(ObjectType::Player);
		Initialize();
	}
	~Player()
	{
		for (Tail* tail : m_tails) {
			if (!tail) break;
			delete tail;
		}
	}

	virtual void ProcessInputs(KeyInputs& inputs, double elapsedTimeInSec) 
	{
		if (inputs.left || inputs.right) {
			if (inputs.left) m_direction.xAxis = -1;
			else if (inputs.right) m_direction.xAxis = +1;
			m_direction.yAxis = 0;
		}
		else if (inputs.up || inputs.down) {
			if (inputs.up) m_direction.yAxis = +1;
			else if (inputs.down) m_direction.yAxis = -1;
			m_direction.xAxis = 0;
		}
	}
	virtual void Update(double elapsedTimeInSec) 
	{
		UpdatePosition(elapsedTimeInSec);
		UpdateLevelData(elapsedTimeInSec);
		CheckCollisionWithTails();
	}
	virtual void Render(Renderer& renderer) {

		int x, y;
		GetPos(&x, &y);
		renderer.PrintOnBuffer(x, y, "P");

		for (Tail* tail : m_tails) {
			if (!tail) break;
			tail->Render(renderer);
		}
	}

	virtual void CollidideWithType(ObjectType type) 
	{
		if (type == ObjectType::Food) {
			m_exp += 10;
		}
	}

	void RenderLevel(Renderer& renderer, int x, int y) 
	{
		char text[30];		
		sprintf_s(text, "Player Level: %d", m_level);

		renderer.PrintOnBuffer(x, y, text);

		RenderLevelUpPopup(renderer, x, y - 1);
	}
	void Initialize() 
	{
		SetPos(0, 0);
		SetIsLived(true);

		m_xMiddlePos = 0.0f;
		m_yMiddlePos = 0.0f;
		memset(&m_direction, 0, sizeof(Direction));

		m_level = 1;
		m_exp = 0;
		m_showLevelUpPopupTimer = 0.0f;
		m_isGameClear = false;

		for (Tail* tail : m_tails) {
			if (!tail) break;
			delete tail;
		}
		m_tails.fill(nullptr);
		m_lastSnake = this;
	}

	bool GetIsGameClear() const {
		return m_isGameClear;
	}

private:
	void RenderLevelUpPopup(Renderer& renderer, int x, int y)
	{
		if (m_showLevelUpPopupTimer > 0) renderer.PrintOnBuffer(x, y, "LEVEL UP!!");
	}
	void CheckCollisionWithTails() 
	{
		for (Tail* tail : m_tails) {
			if (!tail) break;
			
			CheckCollision(*tail);
		}
	}
	void UpdatePosition(double elapsedTimeInSec)
	{
		m_xMiddlePos = m_xMiddlePos
			+ m_direction.xAxis * (PLAYER_DEFAULT_SPEED + PLAYER_SPEED_LEVEL_COEFFICIENT * m_level) * (float)elapsedTimeInSec;
		m_yMiddlePos = m_yMiddlePos
			+ m_direction.yAxis * (PLAYER_DEFAULT_SPEED + PLAYER_SPEED_LEVEL_COEFFICIENT * m_level) * (float)elapsedTimeInSec;

		int x, y;
		GetPos(&x, &y);
		int originalX = x, originalY = y;
		x += (int)m_xMiddlePos;
		y += (int)m_yMiddlePos;
		if (GetIsLived()) SetIsLived(!MoveObjectToWorld(&x, &y));
		else MoveObjectToWorld(&x, &y);
		if (originalX != x || originalY != y) {
			if (originalX != x) m_xMiddlePos = 0.0f;
			if (originalY != y) m_yMiddlePos = 0.0f;

			SaveCurrentPos();
			SetPos(x, y);
			Snake* snake = this;
			for (Tail* tail : m_tails) {
				if (!tail) break;
				tail->Move(*snake);
				snake = tail;
			}
		}
	}
	void UpdateLevelData(double elapsedTimeInSec) 
	{
		int needExp = m_level * NEED_EXP_LEVEL_COEFFICIENT;
		if (needExp <= m_exp) {
			if (m_level == MAX_LEVEL) {
				m_isGameClear = true;
				return;
			}

			m_tails[m_level - 1] = new Tail(*m_lastSnake);
			m_lastSnake = m_tails[m_level - 1];
			m_level++;
			m_exp = 0;
			m_showLevelUpPopupTimer = LEVEL_UP_POP_UP_SHOWING_TIME;
		}

		if (m_showLevelUpPopupTimer > 0) m_showLevelUpPopupTimer -= (float)elapsedTimeInSec;
	}

	float m_xMiddlePos = 0.0f, m_yMiddlePos = 0.0f;

	int m_level;
	int m_exp;
	float m_showLevelUpPopupTimer;
	bool m_isGameClear;

	std::array<Tail*, MAX_LEVEL -1> m_tails = {nullptr, };
	Snake* m_lastSnake;

	struct Direction {
		short xAxis = 0;
		short yAxis = 0;
	} m_direction;
};
