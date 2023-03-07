#include "stdafx.h"
#include "world.h"
#include <random>

void ProcessInputs();
void Update();
void Render();

KeyInputs* g_keyInputs;
World* g_world;

int main() 
{
	g_keyInputs = new KeyInputs();
	g_world = new World();

	while (true) {
		ProcessInputs();
		Update();
		Render();
	}

	delete g_world;
	delete g_keyInputs;
}

void ProcessInputs()
{
	memset(g_keyInputs, 0, sizeof(KeyInputs));

	if (GetAsyncKeyState(VK_LEFT))	g_keyInputs->left = true;
	if (GetAsyncKeyState(VK_RIGHT)) g_keyInputs->right = true;
	if (GetAsyncKeyState(VK_UP))	g_keyInputs->up = true;
	if (GetAsyncKeyState(VK_DOWN))	g_keyInputs->down = true;
	if (GetAsyncKeyState(VK_SPACE))	g_keyInputs->spacebar = true;

	g_world->ProcessInputs(*g_keyInputs);
}

void Update()
{
	g_world->Update();
}

void Render()
{
	system("cls");

	g_world->Render();
}
