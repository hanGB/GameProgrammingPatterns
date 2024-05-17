#pragma once
#include "stdafx.h"
#include "per_game.h"
#include "console_logger.h"
#include "event_dispatcher.h"

inline int CalculateDeltaTime(std::chrono::system_clock::time_point* lastTime, std::chrono::system_clock::time_point* currentTime);
inline void SleepForRestDevice(int dTime);

void CreateWorkerThreads(DWORD threadID);

DWORD WINAPI GameTheadFunc(LPVOID temp);
DWORD WINAPI RenderTheadFunc(LPVOID temp);
DWORD WINAPI UIUpdateTheadFunc(LPVOID temp);
DWORD WINAPI UIRenderTheadFunc(LPVOID temp);
DWORD WINAPI AudioTheadFunc(LPVOID temp);
DWORD WINAPI EventDispatchterTheadFunc(LPVOID temp);
DWORD WINAPI LogTheadFunc(LPVOID temp);