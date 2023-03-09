#include "stdafx.h"
#include "world.h"
#include "renderer.h"
#include <chrono>
#include <thread>

void ProcessInputs();
void Update();
void Render();

KeyInputs* g_keyInputs;
Renderer* g_renderer;
World* g_world;

int main() 
{
	g_keyInputs = new KeyInputs();
	g_renderer = new Renderer();
	g_world = new World();

	while (true) {
		auto startTime = std::chrono::high_resolution_clock::now();

		ProcessInputs();
		Update();
		Render();
		
		auto durationTime = std::chrono::high_resolution_clock::now() - startTime;
		int sleepTime = MILLISECOND_PER_FRAME - (int)std::chrono::duration_cast<std::chrono::milliseconds>(durationTime).count();
		if (sleepTime > 0) {
			std::this_thread::sleep_for(std::chrono::milliseconds((int)sleepTime));
		}
	}

	delete g_world;
	delete g_renderer;
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
	g_world->Render(*g_renderer);
	g_renderer->FlipBuffer();
}
