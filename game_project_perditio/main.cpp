#include "stdafx.h"
#include "per_game.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class";
LPCTSTR lpszWindowName = L"Perditio";

HWND g_hWnd;
HANDLE g_hGameLoopThread;
PERGame* g_game;
bool g_isGameEnd;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
DWORD WINAPI GameLoopTheadFuc(LPVOID temp);

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
		PER_WINDOW_LOCATION_X, PER_WINDOW_LOCATION_Y, 
		PER_WINDOW_WIDTH, PER_WINDOW_HEIGHT, 
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
		AllocConsole();
		FILE* stream;
		freopen_s(&stream, "CONIN$", "r", stdin);
		freopen_s(&stream, "CONOUT$", "w", stderr);
		freopen_s(&stream, "CONOUT$", "w", stdout);
#endif 
		g_game = new PERGame();
		g_isGameEnd = false;
		g_hGameLoopThread = CreateThread(NULL, 0, GameLoopTheadFuc, NULL, 0, &threadID);
		break;
	
	case WM_KEYDOWN:
		// esc를 누를 경우 종료
		if (wParam == VK_ESCAPE) PostMessageW(hWnd, WM_DESTROY, 0, 0);

		g_game->HandleInput(wParam, true);
		break;

	case WM_KEYUP:
		g_game->HandleInput(wParam, false);
		break;

	case WM_DESTROY:
		g_isGameEnd = true;
		CloseHandle(g_hGameLoopThread);
		delete g_game;
#ifdef PER_DEBUG
		FreeConsole();
#endif
		PostQuitMessage(0);
		break;
	}
	// 위의 세 메시지 외의 나머지 메세지는 OS로
	return DefWindowProc(hWnd, uMsg, wParam, lParam);    
}

DWORD WINAPI GameLoopTheadFuc(LPVOID temp)
{
	auto lastTime = std::chrono::system_clock::now();
	while (!g_isGameEnd) {
		auto currentTime = std::chrono::system_clock::now();
		auto deltaTime = currentTime - lastTime;

		// 경과된 시간(마이크로초 단위)
		int dTime = (int)std::chrono::duration_cast<std::chrono::microseconds>(deltaTime).count();

		// 현재 시간을 마지막 시간으로 저장
		lastTime = currentTime;

		g_game->Update(dTime);
		g_game->Render(g_hWnd);
	}

	return 0;
}
