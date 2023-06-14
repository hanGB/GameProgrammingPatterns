#pragma once
#include "per_world.h"

class PERRenderer {
public:
	PERRenderer();
	~PERRenderer();

	// 현재 윈도우 사이즈와 메모리DC 설정
	void MatchWindowSizeAndCurrentMemoryDC(HWND hWnd, HDC memDC);
	// 게임 월드 렌더링
	void RenderWorld(PERWorld& world);
	// 모양 하나 렌더링
	void RenderShape(PERShapeType type, PERVec3 pos, PERVec2 size, PERColor color);

private:
	// 각종 모양 렌더링
	void RenderEllipse(PERVec3 pos, PERVec2 size);
	void RenderRectangle(PERVec3 pos, PERVec2 size);
	void RenderTriangle(PERVec3 pos, PERVec2 size);

	// openGL과 윈도우즈 좌표계간 변환
	void ConvertCoordinateOpenGLToWindows(double* x, double* y);
	void ConvertCoordinateWindowsToOpenGL(double* x, double* y);
	PERVec3 ConvertCoordinateOpenGLToWindowsForVec3(PERVec3 vec);
	PERVec2 ConvertCoordinateOpenGLToWindowsForVec2(PERVec2 vec);

	// 화면 크기와 비율에 맞추어 사이즈 변환
	void MatchSizeWithScreenSizeAndRatio(double* x, double* y);
	PERVec3 MatchSizeWithScreenSizeAndRatioForVec3(PERVec3 vec);
	PERVec2 MatchSizeWithScreenSizeAndRatioForVec2(PERVec2 vec);

	double m_halfWidth;
	double m_halfHeight;

	HDC m_memoryDC;
};