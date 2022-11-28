#include "stdafx.h"
#include "scene.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Game Programming Patterns";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
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
	hWnd = CreateWindow(lpszClass, lpszWindowName,
		WS_OVERLAPPEDWINDOW, 200, 100, WINDOW_SIZE, WINDOW_SIZE, NULL, (HMENU)NULL, hInstance, NULL);

	// 윈도우 출력
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// 이벤트 루프 처리
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

Scene* g_scene;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC, memDC;
	HBITMAP hBitmap, oldBitmap;
	RECT rect;

	// 메세지 처리하기
	switch (uMsg) {
	case WM_CREATE:
		g_scene = new Scene();

#ifdef DEBUG
		AllocConsole();
		FILE* stream;
		freopen_s(&stream, "CONIN$", "r", stdin);
		freopen_s(&stream, "CONOUT$", "w", stderr);
		freopen_s(&stream, "CONOUT$", "w", stdout);
#endif 

		SetTimer(hWnd, 1, GPP_TIME_PER_FRAME, NULL);
		SetTimer(hWnd, 2, GPP_TIME_PER_FRAME, NULL);
		break;

	case WM_KEYDOWN:

		break;

	case WM_KEYUP:

		break;

	case WM_TIMER:
		switch (wParam) {
		case 1:
			g_scene->Draw();
			break;
		case 2:
			InvalidateRect(hWnd, NULL, false);
			break;
		}	
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		// 더블 버퍼링 관련 설정
		GetClientRect(hWnd, &rect);
		memDC = CreateCompatibleDC(hDC);
		hBitmap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
		oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

		// 전체 흰색 초기화
		Rectangle(memDC, -1, -1, rect.right + 1, rect.bottom + 1);

		char pixels[(WINDOW_SIZE / PIXEL_SIZE) * (WINDOW_SIZE / PIXEL_SIZE)];
		memcpy(pixels, g_scene->GetBuffer().GetPixels(), sizeof(pixels));

		for (int i = 0; i < (WINDOW_SIZE / PIXEL_SIZE) * (WINDOW_SIZE / PIXEL_SIZE); ++i) {
			HPEN hPen, oldPen;
			HBRUSH hBrush, oldBrush;
			
			hPen = CreatePen(PS_SOLID, 0, RGB(pixels[i], pixels[i], pixels[i]));
			hBrush = CreateSolidBrush(RGB(pixels[i], pixels[i], pixels[i]));
			oldPen = (HPEN)SelectObject(memDC, hPen);
			oldBrush = (HBRUSH)SelectObject(memDC, hBrush);

			int x = i % (WINDOW_SIZE / PIXEL_SIZE);
			int y = i / (WINDOW_SIZE / PIXEL_SIZE);

			Rectangle(memDC, x * PIXEL_SIZE, y * PIXEL_SIZE, x * PIXEL_SIZE + PIXEL_SIZE, y * PIXEL_SIZE + PIXEL_SIZE);

			(HPEN)SelectObject(memDC, oldPen);
			(HBRUSH)SelectObject(memDC, oldBrush);

			DeleteObject(oldPen);
			DeleteObject(oldBrush);
			DeleteObject(hPen);
			DeleteObject(hBrush);
		}

		// 실제 출력 버퍼로 이동
		BitBlt(hDC, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);
		
		// 더블 버퍼링관련 데이터 삭제
		DeleteObject(SelectObject(memDC, oldBitmap));
		DeleteDC(memDC);
		DeleteDC(hDC);
		DeleteObject(hBitmap);
		DeleteObject(oldBitmap);

		EndPaint(hWnd, &ps);

		break;
	case WM_DESTROY:
		delete g_scene;

#ifdef DEBUG
		FreeConsole();
#endif
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);    // 위의 세 메시지 외의 나머지 메세지는 OS로
}