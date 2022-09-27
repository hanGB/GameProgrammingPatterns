#include "stdafx.h"
#include "entity.h"
#include "achievements.h"
#include "physics.h"

int main()
{
	Achievements* achievement = new Achievements();
	Physics* physics = new Physics();
	physics->AddObserver(achievement);

	Entity* hero = new Entity();
	hero->SetIsHero(true);

	while (true) {
		if (GetAsyncKeyState(VK_LEFT)) {
			hero->MoveWithAxisX(-1, 0.1f);
		}
		if (GetAsyncKeyState(VK_RIGHT)) {
			hero->MoveWithAxisX(1, 0.1f);
		}

		physics->SetElapsedTime(0.032f);
		physics->UpdateEntiy(*hero);

		hero->Render();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	physics->RemoveObserver(achievement);

	delete hero;
	delete physics;
	delete achievement;
}