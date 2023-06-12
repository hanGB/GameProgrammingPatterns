#include "stdafx.h"
#include "per_game.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class";
LPCTSTR lpszWindowName = L"Perditio";

HWND g_hWnd;
HANDLE g_hGameLoopThread;
PerGame* g_game;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
DWORD WINAPI GameLoopTheadFuc(LPVOID temp);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	// ������ Ŭ���� ����ü �� ����
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

	// ������ Ŭ���� ���
	RegisterClassEx(&WndClass);

	// ������ ����
	g_hWnd = CreateWindow(lpszClass, lpszWindowName,
		WS_OVERLAPPEDWINDOW, 
		PER_WINDOW_LOCATION_X, PER_WINDOW_LOCATION_Y, 
		PER_WINDOW_WIDTH, PER_WINDOW_HEIGHT, 
		NULL, (HMENU)NULL, hInstance, NULL);

	// ������ ���
	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	// �̺�Ʈ ���� ó��
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	DWORD threadID;

	// �޼��� ó���ϱ�
	switch (uMsg) {
	case WM_CREATE:
#ifdef PER_DEBUG
		AllocConsole();
		FILE* stream;
		freopen_s(&stream, "CONIN$", "r", stdin);
		freopen_s(&stream, "CONOUT$", "w", stderr);
		freopen_s(&stream, "CONOUT$", "w", stdout);
#endif 
		g_game = new PerGame();
		g_hGameLoopThread = CreateThread(NULL, 0, GameLoopTheadFuc, NULL, 0, &threadID);
		break;
	
	case WM_KEYDOWN:
		g_game->HandleInput(wParam, true);
		break;

	case WM_DESTROY:
		CloseHandle(g_hGameLoopThread);
		delete g_game;
#ifdef PER_DEBUG
		FreeConsole();
#endif
		PostQuitMessage(0);
		break;
	}
	// ���� �� �޽��� ���� ������ �޼����� OS��
	return DefWindowProc(hWnd, uMsg, wParam, lParam);    
}

DWORD WINAPI GameLoopTheadFuc(LPVOID temp)
{
	auto lastTime = std::chrono::system_clock::now();
	while (true) {
		auto currentTime = std::chrono::system_clock::now();
		auto deltaTime = currentTime - lastTime;

		// ����� �ð�(����ũ���� ����)
		int dTime = (int)std::chrono::duration_cast<std::chrono::microseconds>(deltaTime).count();

		// ���� �ð��� ������ �ð����� ����
		lastTime = currentTime;

		g_game->Update(dTime);
		g_game->Render(g_hWnd);
	}
}