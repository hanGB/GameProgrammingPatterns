#pragma once
#include "stdafx.h"
#include "snake.h"
#include "tail.h"
#include <array>

class Player : public Snake {
public:
	Player() { 
		SetType(ObjectType::Player);
		Initialize();
	}
	~Player() {
	
		for (Tail* tail : m_tails) {
			if (!tail) break;
			delete tail;
		}
	}

	virtual void ProcessInputs(KeyInputs& inputs) {

		int x, y;
		GetPos(&x, &y);
		int originalX = x, originalY = y;

		if (inputs.left) x--;
		if (inputs.right) x++;
		if (inputs.up) y++;
		if (inputs.down) y--;

		if (GetIsLived()) SetIsLived(!MoveObjectToWorld(&x, &y));
		else MoveObjectToWorld(&x, &y);
		if (originalX != x || originalY != y) {
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
	virtual void Update() {

		int needExp = m_level * NEED_EXP_LEVEL_COEFFICIENT;
		if (needExp <= m_exp) {
			m_tails[m_level - 1] = new Tail(*m_lastSnake);
			m_lastSnake = m_tails[m_level - 1];
			m_level++;
			m_exp = 0;
			m_showLevelUpPopupTimer = LEVEL_UP_POP_UP_SHOWING_TIME;
		}

		if (m_showLevelUpPopupTimer > 0) m_showLevelUpPopupTimer--;

		CheckCollitionWithTails();
	}
	virtual void Render() {

		int x, y;
		GetPos(&x, &y);
		MoveCurser(x, y);
		std::cout << "P";

		for (Tail* tail : m_tails) {
			if (!tail) break;
			tail->Render();
		}
	}

	virtual void CollidideWithType(ObjectType type) {

		if (type == ObjectType::Food) {
			m_exp += 10;
		}
	}

	void RenderLevel(int x, int y) {

		MoveCurser(x, y);
		std::cout << "Player Level: " << m_level;
		RenderLevelUpPopup(x, y - 1);
	}
	void Initialize() {

		SetPos(0, 0);
		SetIsLived(true);

		m_level = 1;
		m_exp = 0;
		m_showLevelUpPopupTimer = 0;

		for (Tail* tail : m_tails) {
			if (!tail) break;
			delete tail;
		}
		m_tails.fill(nullptr);
		m_lastSnake = this;
	}

private:
	void RenderLevelUpPopup(int x, int y) {

		MoveCurser(x, y);
		if (m_showLevelUpPopupTimer > 0) std::cout << "LEVEL UP!!";
	}
	void CheckCollitionWithTails() {

		for (Tail* tail : m_tails) {
			if (!tail) break;
			
			CheckCollision(*tail);
		}
	}

	int m_level;
	int m_exp;
	int m_showLevelUpPopupTimer;

	std::array<Tail*, 100> m_tails = {nullptr, };
	Snake* m_lastSnake;
};
