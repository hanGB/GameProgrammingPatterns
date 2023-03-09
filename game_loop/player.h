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

		if (m_showLevelUpPopupTimer > 0) m_showLevelUpPopupTimer--;

		CheckCollitionWithTails();
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

	virtual void CollidideWithType(ObjectType type) {

		if (type == ObjectType::Food) {
			m_exp += 10;
		}
	}

	void RenderLevel(Renderer& renderer, int x, int y) {

		char text[30];		
		sprintf_s(text, "Player Level: %d", m_level);

		renderer.PrintOnBuffer(x, y, text);

		RenderLevelUpPopup(renderer, x, y - 1);
	}
	void Initialize() {

		SetPos(0, 0);
		SetIsLived(true);

		m_level = 1;
		m_exp = 0;
		m_showLevelUpPopupTimer = 0;
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
	void RenderLevelUpPopup(Renderer& renderer, int x, int y) {
		
		if (m_showLevelUpPopupTimer > 0) renderer.PrintOnBuffer(x, y, "LEVEL UP!!");
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
	bool m_isGameClear;

	std::array<Tail*, MAX_LEVEL -1> m_tails = {nullptr, };
	Snake* m_lastSnake;
};
