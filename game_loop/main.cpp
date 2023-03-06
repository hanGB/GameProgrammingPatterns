#include "stdafx.h"
#include "player.h"

void ProcessInputs();
void Update();
void Render();
void RenderWorld();

Player* g_player;
KeyInputs* g_keyInputs;

int main() 
{
	g_player = new Player();
	g_keyInputs = new KeyInputs();

	while (true) {
		ProcessInputs();
		Update();
		Render();
	}

	delete g_player;
	delete g_keyInputs;
}

void ProcessInputs()
{
	memset(g_keyInputs, 0, sizeof(KeyInputs));

	if (GetAsyncKeyState(VK_LEFT))	g_keyInputs->left = true;
	if (GetAsyncKeyState(VK_RIGHT)) g_keyInputs->right = true;
	if (GetAsyncKeyState(VK_UP))	g_keyInputs->up = true;
	if (GetAsyncKeyState(VK_DOWN))	g_keyInputs->down = true;

	g_player->ProcessInputs(*g_keyInputs);
}

void Update()
{
	g_player->Update();
}

void Render()
{
	system("cls");
	RenderWorld();

	g_player->Render();
	g_player->RenderLevel();
}

void RenderWorld() 
{
	int y = WORLD_HEIGHT;
	MoveCurser(-WORLD_WIDTH, y);
	std::cout << "######################";

	for (y; y > (-WORLD_HEIGHT); --y) {
		MoveCurser(-WORLD_WIDTH, y);
		std::cout << '#';
		MoveCurser(WORLD_WIDTH, y);
		std::cout << '#';
	}
	MoveCurser(-WORLD_WIDTH, y);
	std::cout << "#######################";
}