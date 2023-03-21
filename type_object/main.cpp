#include "stdafx.h"
#include "breed.h"
#include "monster.h"

int main()
{
	Breed* dragonBreed = new Breed(nullptr, 230, "용이 불을 뿜습니다!");
	Breed* trollBreed = new Breed(nullptr, 48, "트롤이 당신을 공봉으로 내리칩니다!");

	Breed* greatDragonBreed = new Breed(dragonBreed, 460, nullptr);

	Monster* monsters[] = {
		dragonBreed->NewMonster(),
		greatDragonBreed->NewMonster(),
		trollBreed->NewMonster()
	};


	for (auto& monster : monsters) {
		std::cout << "체력이 " << monster->GetHealth() << "인 ";
		std::cout << monster->GetAttack() << std::endl;
	}


	for (auto& monster : monsters) {
		delete monster;
	}
	delete dragonBreed;
	delete trollBreed;
}