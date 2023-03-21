#include "stdafx.h"
#include "breed.h"
#include "monster.h"

int main()
{
	Breed* dragonBreed = new Breed(nullptr, 230, "���� ���� �ս��ϴ�!");
	Breed* trollBreed = new Breed(nullptr, 48, "Ʈ���� ����� �������� ����Ĩ�ϴ�!");

	Breed* greatDragonBreed = new Breed(dragonBreed, 460, nullptr);

	Monster* monsters[] = {
		dragonBreed->NewMonster(),
		greatDragonBreed->NewMonster(),
		trollBreed->NewMonster()
	};


	for (auto& monster : monsters) {
		std::cout << "ü���� " << monster->GetHealth() << "�� ";
		std::cout << monster->GetAttack() << std::endl;
	}


	for (auto& monster : monsters) {
		delete monster;
	}
	delete dragonBreed;
	delete trollBreed;
}