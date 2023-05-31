#pragma once
#include "stdafx.h"

class LootDrop;

class AIComponent {
public:
	void Update(double time) 
	{
		//std::cout << "Ai update\n";
		elapsedTime += time;

		if (delay < elapsedTime) {
			target++;
			elapsedTime = 0;
		}
	}

private:
	int target = 0;
	double delay = 1.0;

	double elapsedTime = 0.0;

	// 자주 사용하지 않는 부분 외부 클래스 분리
	LootDrop* loot;
};

class LootDrop {
	friend class AIComponent;

private:
	int minDrops;
	int maxDrops;
	double chanceOfDrop;
};

class PhysicsComponent {
public:
	void Update(double time)
	{
		//std::cout << "Physics update\n";
		distance += time * 10;
	}

private:
	int mass = 10;
	int sizeX = 10, sizeY = 10;
	int speed = 10;

	double distance = 0.0;
};

class RenderComponent {
public:
	void Render()
	{
		//std::cout << "Render\n";
		renderCount++;
	}

private:
	int mesh = 0;
	int texture = 0;
	int shader = 0;

	int renderCount = 0;
};