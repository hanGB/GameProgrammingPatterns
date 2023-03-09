#include "stdafx.h"
#include "world.h"
#include "renderer.h"
#include <chrono>
#include <thread>

void ProcessInputs(double elapsedTimeInSec);
void Update(double elapsedTimeInSec);
void Render();

KeyInputs* g_keyInputs;
Renderer* g_renderer;
World* g_world;

std::chrono::steady_clock::time_point g_lastTime;

int main() 
{
	g_keyInputs = new KeyInputs();
	g_renderer = new Renderer();
	g_world = new World();
	g_lastTime = std::chrono::high_resolution_clock::now();
	while (true) {
		auto current = std::chrono::high_resolution_clock::now();
		auto durationTime = current - g_lastTime;
		double elapsedTimeInSec = (double)std::chrono::duration_cast<std::chrono::milliseconds>(durationTime).count() / 1'000;

		ProcessInputs(elapsedTimeInSec);
		Update(elapsedTimeInSec);
		Render();
		
		g_lastTime = current;
	}

	delete g_world;
	delete g_renderer;
	delete g_keyInputs;
}

void ProcessInputs(double elapsedTimeInSec)
{
	memset(g_keyInputs, 0, sizeof(KeyInputs));

	if (GetAsyncKeyState(VK_LEFT))	g_keyInputs->left = true;
	if (GetAsyncKeyState(VK_RIGHT)) g_keyInputs->right = true;
	if (GetAsyncKeyState(VK_UP))	g_keyInputs->up = true;
	if (GetAsyncKeyState(VK_DOWN))	g_keyInputs->down = true;
	if (GetAsyncKeyState(VK_SPACE))	g_keyInputs->spacebar = true;

	g_world->ProcessInputs(*g_keyInputs, elapsedTimeInSec);
}
void Update(double elapsedTimeInSec)
{
	g_world->Update(elapsedTimeInSec);
}
void Render()
{
	g_world->Render(*g_renderer);
	g_renderer->FlipBuffer();
}
