#include "stdafx.h"
#include "window_proc_with_thread.h"

HINSTANCE g_hInst;
HWND g_hWnd;
HANDLE g_hWorkerThreads[PER_NUM_WORKER_THREAD];
PERGame* g_game;
bool g_isGameEnd;

inline int CalculateDeltaTime(std::chrono::system_clock::time_point* lastTime, std::chrono::system_clock::time_point* currentTime)
{
	// 현재 시간 측정
	*currentTime = std::chrono::system_clock::now();
	auto deltaTime = *currentTime - *lastTime;

	// 경과된 시간(마이크로초 단위)
	int dTime = (int)std::chrono::duration_cast<std::chrono::microseconds>(deltaTime).count();

	// 현재 시간을 마지막 시간으로 저장
	*lastTime = *currentTime;

	return dTime;
}

inline void SleepForRestDevice(int dTime)
{
	// 너무 빠를 경우 휴식
	int restTime = PER_MINIMUM_FRAME_TIME - dTime;
	if (restTime > 0) {
		Sleep(int(restTime / 1'000.0));
	}
}

void CreateWorkerThreads(DWORD threadID)
{
	g_hWorkerThreads[0] = CreateThread(NULL, 0, GameTheadFunc, NULL, 0, &threadID);
	g_hWorkerThreads[1] = CreateThread(NULL, 0, RenderTheadFunc, NULL, 0, &threadID);
	g_hWorkerThreads[2] = CreateThread(NULL, 0, UIUpdateTheadFunc, NULL, 0, &threadID);
	g_hWorkerThreads[3] = CreateThread(NULL, 0, UIRenderTheadFunc, NULL, 0, &threadID);
	g_hWorkerThreads[4] = CreateThread(NULL, 0, AudioTheadFunc, NULL, 0, &threadID);
	g_hWorkerThreads[5] = CreateThread(NULL, 0, EventDispatchterTheadFunc, NULL, 0, &threadID);
	g_hWorkerThreads[6] = CreateThread(NULL, 0, LogTheadFunc, NULL, 0, &threadID);
}

DWORD WINAPI GameTheadFunc(LPVOID temp)
{
	PERLog::Logger().Info("게임 스레드 시작");

	int dTime;
	auto lastTime = std::chrono::system_clock::now(); auto currentTime = std::chrono::system_clock::now();
	while (!g_isGameEnd) {
		dTime = CalculateDeltaTime(&lastTime, &currentTime);
		g_game->Update(dTime);
		SleepForRestDevice(dTime);
	}

	PERLog::Logger().Info("게임 스레드 종료");
	return 0;
}

DWORD WINAPI RenderTheadFunc(LPVOID temp)
{
	PERLog::Logger().Info("렌더 스레드 시작");

	int dTime;
	auto lastTime = std::chrono::system_clock::now(); auto currentTime = std::chrono::system_clock::now();
	while (!g_isGameEnd) {
		dTime = CalculateDeltaTime(&lastTime, &currentTime);
		g_game->Render(g_hWnd);
		SleepForRestDevice(dTime);
	}

	PERLog::Logger().Info("렌더 스레드 종료");
	return 0;
}

DWORD WINAPI UIUpdateTheadFunc(LPVOID temp)
{
	PERLog::Logger().Info("UI 업데이트 스레드 시작");

	int dTime;
	auto lastTime = std::chrono::system_clock::now(); auto currentTime = std::chrono::system_clock::now();
	while (!g_isGameEnd) {
		dTime = CalculateDeltaTime(&lastTime, &currentTime);
		g_game->UIUpdate(dTime);
		SleepForRestDevice(dTime);
	}

	PERLog::Logger().Info("UI 업데이트 스레드 종료");
	return 0;
}
DWORD WINAPI UIRenderTheadFunc(LPVOID temp)
{
	PERLog::Logger().Info("UI 렌더 스레드 시작");

	int dTime;
	auto lastTime = std::chrono::system_clock::now(); auto currentTime = std::chrono::system_clock::now();
	while (!g_isGameEnd) {
		dTime = CalculateDeltaTime(&lastTime, &currentTime);
		g_game->UIRender(g_hWnd);
		SleepForRestDevice(dTime);
	}

	PERLog::Logger().Info("UI 렌더 스레드 종료");
	return 0;
}

DWORD WINAPI AudioTheadFunc(LPVOID temp)
{
	PERLog::Logger().Info("오디오 스레드 시작");

	int dTime;
	auto lastTime = std::chrono::system_clock::now(); auto currentTime = std::chrono::system_clock::now();
	while (!g_isGameEnd) {
		dTime = CalculateDeltaTime(&lastTime, &currentTime);
		g_game->AudioUpdate();
		SleepForRestDevice(dTime);
	}

	PERLog::Logger().Info("오디오 스레드 종료");
	return 0;
}

DWORD WINAPI EventDispatchterTheadFunc(LPVOID temp)
{
	PERLog::Logger().Info("이벤트 디스패쳐 스레드 시작");

	int dTime;
	auto lastTime = std::chrono::system_clock::now(); auto currentTime = std::chrono::system_clock::now();
	while (!g_isGameEnd) {
		dTime = CalculateDeltaTime(&lastTime, &currentTime);
		EventDispatcher::Update();
		g_game->DoWindowJob(g_hWnd);
		SleepForRestDevice(dTime);
	}

	PERLog::Logger().Info("이벤트 디스패쳐 스레드 종료");
	return 0;
}

DWORD WINAPI LogTheadFunc(LPVOID temp)
{
	PERLog::Logger().Info("로그 스레드 시작");

	int dTime;
	auto lastTime = std::chrono::system_clock::now(); auto currentTime = std::chrono::system_clock::now();
	while (!g_isGameEnd) {
		dTime = CalculateDeltaTime(&lastTime, &currentTime);
		PERLog::Logger().Update();
		SleepForRestDevice(dTime);
	}

	PERLog::Logger().Info("로그 스레드 종료");
	return 0;
}