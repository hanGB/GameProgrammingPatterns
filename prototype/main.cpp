#include "stdafx.h"
#include "monster.h"
#include "spawner.h"

int main()
{
	auto lastTime = std::chrono::system_clock::now();

	Spawner* spawners[] = { 
		new Spawner(new Ghost(), 1.0f), 
		new Spawner(new Demon(), 1.5f), 
		new Spawner(new Sorcerer(), 2.0f) 
	};

	while (true) {
		auto nowTime = std::chrono::system_clock::now();
		auto elapsedTimeInSec = std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - lastTime).count() / 1'000'000'000.0f;
		lastTime = nowTime;
		for (auto spawner : spawners) {
			auto monster = spawner->SpawnMonster(elapsedTimeInSec);
			if (monster) {
				monster->Render();
				std::cout << '\n';
				delete monster;
			}
		}

		if (GetAsyncKeyState(VK_ESCAPE)) {
			break;
		}
		// std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
	
	for (auto spawner : spawners) {
		delete spawner;
	}
}