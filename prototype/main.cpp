#include "stdafx.h"
#include "monster.h"
#include "spawner.h"

int main()
{
	auto lastTime = std::chrono::system_clock::now();

	Spawner* spawners[] = { 
		new Spawner(new Ghost(10, 20), 1.0f), 
		new Spawner(new Ghost(50, 30), 3.0f),
		new Spawner(new Ghost(100, 0), 5.0f),
		new Spawner(new Demon(), 1.5f), 
		new Spawner(new Sorcerer(), 2.0f) 
	};

	float time = 0.0f;

	while (true) {
		auto nowTime = std::chrono::system_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - lastTime).count();
		lastTime = nowTime;
		for (auto spawner : spawners) {
			auto monster = spawner->SpawnMonster(elapsedTime / MILLISEC_PER_SEC);
			if (monster) {
				monster->Render();
				delete monster;
			}
		}

		if (GetAsyncKeyState(VK_ESCAPE)) {
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(FPS_120_MS));
	}
	
	for (auto spawner : spawners) {
		delete spawner;
	}
}