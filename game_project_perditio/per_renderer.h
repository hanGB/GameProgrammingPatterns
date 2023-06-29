#pragma once

class PERRenderer {
public:
	PERRenderer(HWND hWnd);
	~PERRenderer();

	// 현재 윈도우 사이즈와 메모리DC 설정
	void MatchWindowSize(HWND hWnd);

	void SetCameraPosition(PERVec2 pos);
	void SetCameraSightWidth(double width);
	
	// 메모리DC 초기화, 비트맵 생성
	void ResetMemoryDC(HWND hWnd);
	void ResetUIMemoryDC(HWND hWnd);
	// 실제 HDC로 이동
	void FillHDCWithMemoryDCs(HWND hWnd);

	// 월드 좌표계 렌더링
	void RenderShapeInWorldCoordinate(PERShapeType type, PERVec3 pos, PERVec3 size, PERColor color, 
		bool border = true, int borderWidth = 1, PERColor borderColor = PERColor(0, 0, 0));
	void RenderFontInWorldCoordinate(const wchar_t* text, int textSize, double size, PERVec2 pos, PERColor color);

	// 스크린 좌표계 렌더링(UI 용)
	void RenderShapeInScreenCoordinate(PERShapeType type, PERVec2 pos, PERVec2 size, PERColor color,
		bool border = true, int borderWidth = 1, PERColor borderColor = PERColor(0, 0, 0));
	void RenderFontInScreenCoordinate(const wchar_t* text, int textSize, double size, PERVec2 posWithRate, PERColor color);

private:
	void RenderShape(PERShapeType type, PERVec2 pos, PERVec2 size, PERColor color,
		bool border, int borderWidth, PERColor borderColor, HDC memDC);
	void RenderFont(const wchar_t* text, int textSize, double size, PERVec2 pos, PERColor color, HDC memDC);

	// 각종 모양 렌더링
	void RenderEllipse(PERVec2 pos, PERVec2 size, HDC memDC);
	void RenderRectangle(PERVec2 pos, PERVec2 size, HDC memDC);
	void RenderRoundRectangle(PERVec2 pos, PERVec2 size, int widthAngle, int heightAngle, HDC memDC);
	void RenderTriangle(PERVec2 pos, PERVec2 size, HDC memDC);
	// 왼쪽, 위 앵커 포인트
	void RenderEllipseWithLeftTopAnchor(PERVec2 pos, PERVec2 size, HDC memDC);
	void RenderRectangleWithLeftTopAnchor(PERVec2 pos, PERVec2 size, HDC memDC);
	void RenderRoundRectangleWithLeftTopAnchor(PERVec2 pos, PERVec2 size, int widthAngle, int heightAngle, HDC memDC);
	void RenderTriangleWithLeftTopAnchor(PERVec2 pos, PERVec2 size, HDC memDC);

	// openGL과 윈도우즈 좌표계간 변환
	// 월드 좌표계에서 변환(-카메라 시야 크키 + 카메라 좌표 ~ 카메라 시야 크키 + 카메라 좌표)
	PERVec2 ConvertWorldCoordinateOpenGLToWindows(PERVec2 pos);
	PERVec2 MatchSizeWithWorldCoordinate(PERVec2 size);
	double MatchSizeWithWorldCoordinate(double size);
	// 스크린 좌표계에서 변환(-1.0 ~ 1.0)
	PERVec2 ConvertScreenCoordinateOpenGLToWindows(PERVec2 pos);
	PERVec2 MatchSizeWithScreenCoordinate(PERVec2 size);
	double MatchSizeWithScreenCoordinate(double size);

	PERVec2 m_cameraPosition = PERVec2(0.0, 0.0);
	double m_cameraSightWidth = 20.0;

	double m_halfWidth;
	double m_halfHeight;

	// 메모리 DC
	HDC m_memoryDC;
	HDC m_uiMemoryDC;
	HBITMAP m_newBitmap, m_oldBitmap;
	HBITMAP m_uiNewBitmap, m_uiOldBitmap;

	// 윈도우 크기 저장
	RECT m_windowSizeRect;
};