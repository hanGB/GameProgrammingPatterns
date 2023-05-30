#include "stdafx.h"
#include "game_entity.h"

int main()
{
	bool gameOver = false;
	int gameEndCounter = 0;

	int numEntities = NUM_OF_GAME_ENTITY;

	GameEntity* entities[NUM_OF_GAME_ENTITY];
	for (int i = 0; i < numEntities; ++i) {
		AIComponent* ai = new AIComponent();
		PhysicsComponent* physics = new PhysicsComponent();
		RenderComponent* render = new RenderComponent();

		GameEntity* gameEntity = new GameEntity(ai, physics, render);

		entities[i] = gameEntity;
	}

	auto lastTime = std::chrono::high_resolution_clock::now();

	while (!gameOver) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = currentTime - lastTime;
		
		double microsecTime = (double)std::chrono::duration_cast<std::chrono::microseconds>(elapsedTime).count();
		double time = microsecTime / 1'000'000.0;
		std::cout << time << std::endl;

		lastTime = currentTime;

		// AI 贸府
		for (int i = 0; i < numEntities; ++i) {
			entities[i]->GetAI()->Update(microsecTime);
		}
		// 拱府 贸府
		for (int i = 0; i < numEntities; ++i) {
			entities[i]->GetPhysics()->Update(microsecTime);
		}
		// 坊歹傅
		for (int i = 0; i < numEntities; ++i) {
			entities[i]->GetRender()->Render();
		}
		gameEndCounter++;

		if (gameEndCounter > GAME_END_LOOP_COUNT) {
			gameOver = true;
		}
	}

	for (int i = 0; i < numEntities; ++i) {
		delete entities[i];
	}
}