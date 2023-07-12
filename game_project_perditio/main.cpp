#include "stdafx.h"
#include "per_game.h"
#include "console_logger.h"
#include "event_dispatcher.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class";
LPCTSTR lpszWindowName = L"Perditio";

HWND g_hWnd;
HANDLE g_hWorkerThreads[PER_NUM_WORKER_THREAD];
PERGame* g_game;
bool g_isGameEnd;
int g_windowSizeW, g_windowSizeH;
bool g_isMaxScreenSize;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
DWORD WINAPI GameTheadFunc(LPVOID temp);
DWORD WINAPI RenderTheadFunc(LPVOID temp);
DWORD WINAPI UIUpdateTheadFunc(LPVOID temp);
DWORD WINAPI UIRenderTheadFunc(LPVOID temp);
DWORD WINAPI AudioTheadFunc(LPVOID temp);
DWORD WINAPI EventDispatchterTheadFunc(LPVOID temp);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	// 윈도우 클래스 구조체 값 설정
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// 윈도우 클래스 등록
	RegisterClassEx(&WndClass);

	// 윈도우 생성
	g_hWnd = CreateWindow(lpszClass, lpszWindowName,
		WS_OVERLAPPEDWINDOW, 
		PER_DEFAULT_WINDOW_LOCATION_X, PER_DEFAULT_WINDOW_LOCATION_Y, 
		PER_DEFAULT_WINDOW_WIDTH, PER_DEFAULT_WINDOW_HEIGHT,
		NULL, (HMENU)NULL, hInstance, NULL);

	// 윈도우 출력
	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	// 이벤트 루프 처리
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	DWORD threadID;

	// 메세지 처리하기
	switch (uMsg) {
	case WM_CREATE:
#ifdef PER_DEBUG
		PERLog::SetLogger(new ConsoleLogger());
#else
		PERLog::SetLogger(nullptr);
#endif 
		g_windowSizeW = PER_DEFAULT_WINDOW_WIDTH;
		g_windowSizeH = PER_DEFAULT_WINDOW_HEIGHT;
		
		// 게임 초기화
		g_game = new PERGame(g_hWnd);
		EventDispatcher::SetGame(dynamic_cast<EventReciver*>(g_game));
		// 월드 실행
		g_game->Recive(PEREvent::EVENT_RUN_DEFAULT_WORLD_AND_GAME_MODE, PERVec3());

		g_isGameEnd = false;
		// 게임 루프 스레드 생성
		g_hWorkerThreads[0] = CreateThread(NULL, 0, GameTheadFunc, NULL, 0, &threadID);
		g_hWorkerThreads[1] = CreateThread(NULL, 0, RenderTheadFunc, NULL, 0, &threadID);
		g_hWorkerThreads[2] = CreateThread(NULL, 0, UIUpdateTheadFunc, NULL, 0, &threadID);
		g_hWorkerThreads[3] = CreateThread(NULL, 0, UIRenderTheadFunc, NULL, 0, &threadID);
		g_hWorkerThreads[4] = CreateThread(NULL, 0, AudioTheadFunc, NULL, 0, &threadID);
		g_hWorkerThreads[5] = CreateThread(NULL, 0, EventDispatchterTheadFunc, NULL, 0, &threadID);
		break;
	
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMaxTrackSize.x = g_windowSizeW;
		((MINMAXINFO*)lParam)->ptMaxTrackSize.y = g_windowSizeH;
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = g_windowSizeW;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = g_windowSizeH;
		break;

	case WM_KEYDOWN:
		// esc를 누를 경우 종료
		if (wParam == VK_ESCAPE) PostMessageW(hWnd, WM_DESTROY, 0, 0);
		if (wParam == VK_RETURN) {
			if (g_isMaxScreenSize) {
				g_windowSizeW = PER_DEFAULT_WINDOW_WIDTH;
				g_windowSizeH = PER_DEFAULT_WINDOW_HEIGHT;
				g_isMaxScreenSize = false;
				SetWindowPos(hWnd, nullptr, PER_DEFAULT_WINDOW_LOCATION_X, PER_DEFAULT_WINDOW_LOCATION_Y, g_windowSizeW, g_windowSizeH, 0);
				g_game->MatchWindowHWND(hWnd);
			}
			else {
				g_windowSizeW = PER_MAXIMUM_WINDOW_WIDTH;
				g_windowSizeH = PER_MAXIMUM_WINDOW_HEIGHT;
				g_isMaxScreenSize = true;
				SetWindowPos(hWnd, nullptr, 0, 0, g_windowSizeW, g_windowSizeH, 0);
				g_game->MatchWindowHWND(hWnd);
			}
		}
		g_game->HandleInput(wParam, true);
		break;

	case WM_KEYUP:
		g_game->HandleInput(wParam, false);
		break;

	case WM_DESTROY:
		g_isGameEnd = true;
		// 게임 루프 스레드가 종료될 때 까지 무한 대기
		WaitForMultipleObjects(PER_NUM_WORKER_THREAD, g_hWorkerThreads, true, INFINITE);

#ifdef PER_DEBUG
		system("pause");
#endif 
		
		PostQuitMessage(0);
		break;
	}
	// 위의 세 메시지 외의 나머지 메세지는 OS로
	return DefWindowProc(hWnd, uMsg, wParam, lParam);    
}

DWORD WINAPI GameTheadFunc(LPVOID temp)
{
	PERLog::Logger().Info("게임 스레드 시작");

	auto lastTime = std::chrono::system_clock::now();
	while (!g_isGameEnd) {
		auto currentTime = std::chrono::system_clock::now();
		auto deltaTime = currentTime - lastTime;

		// 경과된 시간(마이크로초 단위)
		int dTime = (int)std::chrono::duration_cast<std::chrono::microseconds>(deltaTime).count();

		// 현재 시간을 마지막 시간으로 저장
		lastTime = currentTime;

		g_game->Update(dTime);

		// 너무 빠를 경우 휴식
		int restTime = PER_MINIMUM_FRAME_TIME - dTime;
		if (restTime > 0) {
			Sleep(int(restTime / 1'000.0));
		}
	}

	PERLog::Logger().Info("게임 스레드 종료");
	return 0;
}

DWORD WINAPI RenderTheadFunc(LPVOID temp)
{
	PERLog::Logger().Info("렌더 스레드 시작");

	while (!g_isGameEnd) {
		g_game->Render(g_hWnd);
	}

	PERLog::Logger().Info("렌더 스레드 종료");
	return 0;
}

DWORD WINAPI UIUpdateTheadFunc(LPVOID temp)
{
	PERLog::Logger().Info("UI 업데이트 스레드 시작");

	auto lastTime = std::chrono::system_clock::now();
	while (!g_isGameEnd) {
		auto currentTime = std::chrono::system_clock::now();
		auto deltaTime = currentTime - lastTime;

		// 경과된 시간(마이크로초 단위)
		int dTime = (int)std::chrono::duration_cast<std::chrono::microseconds>(deltaTime).count();

		// 현재 시간을 마지막 시간으로 저장
		lastTime = currentTime;

		g_game->UIUpdate(dTime);

		// 너무 빠를 경우 휴식
		int restTime = PER_MINIMUM_FRAME_TIME - dTime;
		if (restTime > 0) {
			Sleep(int(restTime / 1'000.0));
		}
	}

	PERLog::Logger().Info("UI 업데이트 스레드 종료");
	return 0;
}
DWORD WINAPI UIRenderTheadFunc(LPVOID temp)
{
	PERLog::Logger().Info("UI 렌더 스레드 시작");

	while (!g_isGameEnd) {
		g_game->UIRender(g_hWnd);
	}

	PERLog::Logger().Info("UI 렌더 스레드 종료");
	return 0;
}

DWORD WINAPI AudioTheadFunc(LPVOID temp)
{
	PERLog::Logger().Info("오디오 스레드 시작");

	while (!g_isGameEnd) {
		g_game->AudioUpdate();
	}

	PERLog::Logger().Info("오디오 스레드 종료");
	return 0;
}

DWORD WINAPI EventDispatchterTheadFunc(LPVOID temp)
{
	PERLog::Logger().Info("이벤트 디스패쳐 스레드 시작");

	while (!g_isGameEnd) {
		EventDispatcher::Update();
	}

	PERLog::Logger().Info("이벤트 디스패쳐 스레드 종료");
	return 0;
}