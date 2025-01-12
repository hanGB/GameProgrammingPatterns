#pragma once
#include "player.h"
#include "food.h"
#include "renderer.h"
#include <random>

class World {
public:
	World() 
	{
		m_player = new Player();
		m_food = nullptr;

		std::random_device rd;
		m_dre = std::default_random_engine(rd());
		m_uidX = std::uniform_int_distribution<int>(-WORLD_WIDTH, WORLD_WIDTH);
		m_uidY = std::uniform_int_distribution<int>(-WORLD_HEIGHT, WORLD_HEIGHT);

	}
	~World() 
	{ 
		delete m_player;
		if (m_food) delete m_food;
	}

	void ProcessInputs(KeyInputs& inputs, double elapsedTimeInSec)
	{
		if (m_isEnd) 
		{
			if (inputs.spacebar)
			{
				delete m_food;
				m_food = nullptr;
				m_player->Initialize();
				m_isEnd = false;
				m_timer = 0.0;
			}
			return;
		}
		m_player->ProcessInputs(inputs, elapsedTimeInSec);
	}

	void Update(double elapsedTimeInSec)
	{
		if (m_isEnd) return;

		if (!m_player->GetIsLived() || m_player->GetIsGameClear()) {
			m_isEnd = true;
			return;
		}

		if (m_food) {
			if (!m_food->GetIsLived()) {
				delete m_food;
				m_food = nullptr;
			}
		}

		if (m_food) m_food->CheckCollision(*m_player);

		if (m_food) m_food->Update(elapsedTimeInSec);
		m_player->Update(elapsedTimeInSec);

		if (!m_food) MakeFood();

		m_timer += elapsedTimeInSec;
	}

	void Render(Renderer& renderer, double differenceRate)
	{
		m_player->RenderLevel(renderer, -WORLD_WIDTH, -WORLD_HEIGHT - 1);
		RenderTime(renderer);

		if (m_isEnd) {
			if (m_player->GetIsGameClear()) {
				renderer.PrintOnBuffer(0, 0, "Game Clear!!!");
				renderer.PrintOnBuffer(0, -1, "Press space bar to reset...");
				return;
			}
			else {
				renderer.PrintOnBuffer(0, 0, "Game Over!!!");
				renderer.PrintOnBuffer(0, -1, "Press space bar to reset...");
				return;
			}
		}
		RenderWall(renderer);

		if (m_food) m_food->Render(renderer, differenceRate);
		m_player->Render(renderer, differenceRate);
	}

private:
	void MakeFood()
	{
		int x = m_uidX(m_dre);
		int y = m_uidY(m_dre);
		MoveObjectToWorld(&x, &y);

		m_food = new Food(x, y);
	}

	void RenderWall(Renderer& renderer)
	{
		int y = WORLD_HEIGHT;
		for (int x = -WORLD_WIDTH; x <= WORLD_WIDTH; ++x) {
			renderer.PrintOnBuffer(x, y, "#");
		}	
		for (y; y > (-WORLD_HEIGHT); --y) {
			renderer.PrintOnBuffer(-WORLD_WIDTH, y, "#");
			renderer.PrintOnBuffer(WORLD_WIDTH, y, "#");
		}
		for (int x = -WORLD_WIDTH; x <= WORLD_WIDTH; ++x) {
			renderer.PrintOnBuffer(x, y, "#");
		}
	}
	void RenderTime(Renderer& renderer)
	{
		char text[30];
		sprintf_s(text, "%.3f Seconds", m_timer);
		renderer.PrintOnBuffer(0, -WORLD_HEIGHT -1, text);
	}

	Player* m_player;
	Food* m_food;
	bool m_isEnd = false;
	double m_timer = 0.0;

	std::default_random_engine m_dre;
	std::uniform_int_distribution<int> m_uidX;
	std::uniform_int_distribution<int> m_uidY;
};
