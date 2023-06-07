#include "stdafx.h"
#include "particle_pool.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Game Programming Patterns";

ParticlePool* g_particlePool;
CoordinateData	g_coordinateData;
double			g_spawnGap = 0.0;
struct {
	double x, y;
	bool click = false;
} g_mouseData;

void SpawnParticle() 
{
	double posX = g_mouseData.x;
	double posY = g_mouseData.y;

	g_coordinateData.ConvertCoordinateWindowsToOpenGL(&posX, &posY);

	double oneDegreeInRadian = PI_VALUE / 2.0 / 90.0;

	// 원형으로 퍼지는 파티클 생성
	for (int i = 0; i < 360; i += 10) {
		double dirX = cos(oneDegreeInRadian * i);
		double dirY = sin(oneDegreeInRadian * i);

		g_particlePool->Craete(
			posX, posY, 
			DEFUALT_PARTICLE_SPEED * dirX, DEFUALT_PARTICLE_SPEED * dirY, 
			DEFUALT_PARTICLE_LIFE_TIME
		);
	}
}


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
		WS_OVERLAPPEDWINDOW, 200, 100, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, (HMENU)NULL, hInstance, NULL);

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC, memDC;
	HBITMAP hBitmap, oldBitmap;
	RECT rect;

	// 메세지 처리하기
	switch (uMsg) {
	case WM_CREATE:
		g_particlePool = new ParticlePool();
		g_coordinateData.halfWidth = WINDOW_WIDTH / 2;
		g_coordinateData.halfHeight = WINDOW_HEIGHT / 2;

#ifdef DEBUG
		AllocConsole();
		FILE* stream;
		freopen_s(&stream, "CONIN$", "r", stdin);
		freopen_s(&stream, "CONOUT$", "w", stderr);
		freopen_s(&stream, "CONOUT$", "w", stdout);
#endif 
		SetTimer(hWnd, 1, TIME_PER_FRAME, NULL);
		break;

	case WM_LBUTTONDOWN:
		if (!g_mouseData.click) {
			g_mouseData.x = (double)LOWORD(lParam);
			g_mouseData.y = (double)HIWORD(lParam);

			SpawnParticle();

			g_mouseData.click = true;
			g_spawnGap = PARTICLE_SPAWN_GAP;
		}
		break;

	case WM_LBUTTONUP:
		if (g_mouseData.click) g_mouseData.click = false;
		break;

	case WM_MOUSEMOVE:
		if (g_mouseData.click) {
			g_mouseData.x = (double)LOWORD(lParam);
			g_mouseData.y = (double)HIWORD(lParam);
		}

		break;

	case WM_TIMER:
		InvalidateRect(hWnd, NULL, false);
		if (g_mouseData.click) {
			g_spawnGap -= (double)TIME_PER_FRAME / 1'000.0;
			if (g_spawnGap <= 0.0) {
				SpawnParticle();
				g_spawnGap = PARTICLE_SPAWN_GAP;
			}
		}

		g_particlePool->Update((double)TIME_PER_FRAME / 1'000.0);
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

		g_particlePool->Render(memDC, g_coordinateData);

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

		delete g_particlePool;

#ifdef DEBUG
		FreeConsole();
#endif
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);    // 위의 세 메시지 외의 나머지 메세지는 OS로
}