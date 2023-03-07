#pragma once
#include "player.h"
#include "food.h"
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

	void ProcessInputs(KeyInputs& inputs)
	{
		if (m_isEnd) 
		{
			if (inputs.spacebar)
			{
				delete m_food;
				m_food = nullptr;
				m_player->Initialize();
				m_isEnd = false;
			}
			return;
		}
		m_player->ProcessInputs(inputs);
	}

	void Update()
	{
		if (m_isEnd) return;

		if (!m_player->GetIsLived()) {
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

		if (m_food) m_food->Update();
		m_player->Update();

		if (!m_food) MakeFood();
	}

	void Render()
	{
		if (m_isEnd) {
			MoveCurser(0, 0);
			std::cout << "GameOver!!!";
			MoveCurser(0, -1);
			std::cout << "Press space bar to reset...";
			m_player->RenderLevel(-WORLD_WIDTH, -WORLD_HEIGHT - 1);
			return;
		}
		RenderWall();

		if (m_food) m_food->Render();
		m_player->Render();
		m_player->RenderLevel(-WORLD_WIDTH, -WORLD_HEIGHT - 1);
	}

private:
	void MakeFood()
	{
		int x = m_uidX(m_dre);
		int y = m_uidY(m_dre);
		MoveObjectToWorld(&x, &y);

		m_food = new Food(x, y);
	}

	void RenderWall()
	{
		int y = WORLD_HEIGHT;
		MoveCurser(-WORLD_WIDTH, y);
		std::cout << "######################";

		for (y; y > (-WORLD_HEIGHT); --y) {
			MoveCurser(-WORLD_WIDTH, y);
			std::cout << '#';
			MoveCurser(WORLD_WIDTH, y);
			std::cout << '#';
		}
		MoveCurser(-WORLD_WIDTH, y);
		std::cout << "#######################";
	}

	Player* m_player;
	Food* m_food;
	bool m_isEnd = false;

	std::default_random_engine m_dre;
	std::uniform_int_distribution<int> m_uidX;
	std::uniform_int_distribution<int> m_uidY;
};
