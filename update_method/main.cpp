#include "stdafx.h"
#include "world.h"

int main()
{
	World* world = new World();

	while (true) {
		world->GameLoop();
	}

	delete world;
}