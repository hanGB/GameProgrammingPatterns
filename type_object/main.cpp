#include "stdafx.h"
#include "breed.h"
#include "monster.h"

int main()
{
	Breed* dragonBreed = new Breed(230, "���� ���� �ս��ϴ�!");
	Breed* trollBreed = new Breed(48, "Ʈ���� ����� �������� ����Ĩ�ϴ�!");

	Monster* monsters[2] = {
		dragonBreed->NewMonster(),
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