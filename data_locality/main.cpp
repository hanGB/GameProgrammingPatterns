#include "stdafx.h"
#include "game_entity.h"

int main()
{
	bool gameOver = false;
	int gameEndCounter = 0;

	int numEntities = NUM_OF_GAME_ENTITY;

	GameEntity* entities[NUM_OF_GAME_ENTITY];
	AIComponent* aiComponents = new AIComponent[NUM_OF_GAME_ENTITY];
	PhysicsComponent* physicsComponents = new PhysicsComponent[NUM_OF_GAME_ENTITY];
	RenderComponent* renderComponents = new RenderComponent[NUM_OF_GAME_ENTITY];

	for (int i = 0; i < numEntities; ++i) {

		GameEntity* gameEntity = new GameEntity(&aiComponents[i], &physicsComponents[i], &renderComponents[i]);

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
			aiComponents[i].Update(microsecTime);
		}
		// 拱府 贸府
		for (int i = 0; i < numEntities; ++i) {
			physicsComponents[i].Update(microsecTime);
		}
		// 坊歹傅
		for (int i = 0; i < numEntities; ++i) {
			renderComponents->Render();
		}
		gameEndCounter++;

		if (gameEndCounter > GAME_END_LOOP_COUNT) {
			gameOver = true;
		}
	}

	for (int i = 0; i < numEntities; ++i) {
		delete entities[i];
	}
	delete[] aiComponents;
	delete[] physicsComponents;
	delete[] renderComponents;
}