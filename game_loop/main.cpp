#include "stdafx.h"
#include "world.h"
#include "renderer.h"
#include <chrono>
#include <thread>

void ProcessInputs(double elapsedTimeInSec);
void Update(double elapsedTimeInSec);
void Render(double differenceRate);

KeyInputs* g_keyInputs;
Renderer* g_renderer;
World* g_world;

std::chrono::steady_clock::time_point g_lastTime;
double g_lag;

int main() 
{
	g_keyInputs = new KeyInputs();
	g_renderer = new Renderer();
	g_world = new World();

	g_lastTime = std::chrono::high_resolution_clock::now();
	g_lag = 0.0;

	// get display data
	DEVMODEA dm = { 0 };
	dm.dmSize = sizeof(dm);
	EnumDisplaySettingsA(nullptr, ENUM_CURRENT_SETTINGS, &dm);

	double displayRefreshRateInMS = 1000.0 / (double)dm.dmDisplayFrequency;

	while (true) {
		auto current = std::chrono::high_resolution_clock::now();
		auto elapsedTime = current - g_lastTime;
		double elapsedTimeInMS = (double)std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime).count();
		g_lastTime = current;
		g_lag += elapsedTimeInMS;
		ProcessInputs(elapsedTimeInMS / 1000.0);

		for (int i = 0; i < MAXIMUM_UPDATE_LOOP_TIME && g_lag >= MILLISECOND_PER_UPDATE; ++i) {
			Update(MILLISECOND_PER_UPDATE / 1'000.0);
			g_lag -= MILLISECOND_PER_UPDATE;
		}
		g_lag = (double)((int)g_lag % (int)MILLISECOND_PER_UPDATE);

		Render(g_lag / MILLISECOND_PER_UPDATE);

		double sleepTime = displayRefreshRateInMS - elapsedTimeInMS;
		if (sleepTime > 0) {
			std::this_thread::sleep_for(std::chrono::milliseconds((int)sleepTime));
		}
		g_renderer->FlipBuffer();
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
void Render(double differenceRate)
{
	g_world->Render(*g_renderer, differenceRate);
}
