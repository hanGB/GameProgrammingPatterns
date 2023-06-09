#include "stdafx.h"
#include "grid.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Game Programming Patterns";

CoordinateData	g_coordinateData;
Grid* g_grid;
Unit* g_units[MAX_UNIT_COUNT];
int g_numUnit = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
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
	hWnd = CreateWindow(lpszClass, lpszWindowName,
		WS_OVERLAPPEDWINDOW, 200, 100, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, (HMENU)NULL, hInstance, NULL);

	// ������ ���
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// �̺�Ʈ ���� ó��
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC, memDC;
	HBITMAP hBitmap, oldBitmap;
	RECT rect;

	// �޼��� ó���ϱ�
	switch (uMsg) {
	case WM_CREATE:
		g_coordinateData.halfWidth = WINDOW_WIDTH / 2;
		g_coordinateData.halfHeight = WINDOW_HEIGHT / 2;

		g_grid = new Grid();

		for (int x = -12; x < 12; ++x) {
			for (int y = -12; y < 12; ++y) {
				Unit* unit = new Unit(g_grid, x * 5 + 2, y * 5 + 2);
				g_units[g_numUnit] = unit;
				g_numUnit++;
			}
		}

#ifdef DEBUG
		AllocConsole();
		FILE* stream;
		freopen_s(&stream, "CONIN$", "r", stdin);
		freopen_s(&stream, "CONOUT$", "w", stderr);
		freopen_s(&stream, "CONOUT$", "w", stdout);
#endif 
		SetTimer(hWnd, 1, TIME_PER_FRAME, NULL);
		break;

	case WM_TIMER:
		InvalidateRect(hWnd, NULL, false);
		g_grid->HandleMelee();

		// ����� ��
		// g_grid->ShowEachCellCount();
		break;

	case WM_KEYDOWN:
		if (wParam == VK_UP) {
			for (int i = 0; i < g_numUnit; ++i) {
				g_units[i]->MoveBy(0, +1);
			}
		}
		if (wParam == VK_DOWN) {
			for (int i = 0; i < g_numUnit; ++i) {
				g_units[i]->MoveBy(0, -1);
			}
		}
		if (wParam == VK_LEFT) {
			for (int i = 0; i < g_numUnit; ++i) {
				g_units[i]->MoveBy(-1, 0);
			}
		}
		if (wParam == VK_RIGHT) {
			for (int i = 0; i < g_numUnit; ++i) {
				g_units[i]->MoveBy(+1, 0);
			}
		}

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		// ���� ���۸� ���� ����
		GetClientRect(hWnd, &rect);
		memDC = CreateCompatibleDC(hDC);
		hBitmap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
		oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

		// ��ü ��� �ʱ�ȭ
		Rectangle(memDC, -1, -1, rect.right + 1, rect.bottom + 1);

		g_grid->RenderGrid(memDC, g_coordinateData);
		for (int i = 0; i < g_numUnit; ++i) {
			g_units[i]->Render(memDC, g_coordinateData);
		}

		// ���� ��� ���۷� �̵�
		BitBlt(hDC, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);

		// ���� ���۸����� ������ ����
		DeleteObject(SelectObject(memDC, oldBitmap));
		DeleteDC(memDC);
		DeleteDC(hDC);
		DeleteObject(hBitmap);
		DeleteObject(oldBitmap);

		EndPaint(hWnd, &ps);

		break;
	case WM_DESTROY:
		for (int i = 0; i < g_numUnit; ++i) {
			delete g_units[i];
		}
		delete g_grid;

#ifdef DEBUG
		FreeConsole();
#endif
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);    // ���� �� �޽��� ���� ������ �޼����� OS��
}