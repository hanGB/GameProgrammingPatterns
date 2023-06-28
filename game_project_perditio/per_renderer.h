#pragma once

class PERRenderer {
public:
	PERRenderer();
	~PERRenderer();

	// ���� ������ ������� �޸�DC ����
	void MatchWindowSizeAndCurrentMemoryDC(HWND hWnd, HDC memDC);

	void SetCameraPosition(PERVec2 pos);
	void SetCameraSightWidth(double width);
	
	// ���� ��ǥ�� ������
	void RenderShapeInWorldCoordinate(PERShapeType type, PERVec3 pos, PERVec3 size, PERColor color, 
		bool border = true, int borderWidth = 1, PERColor borderColor = PERColor(0, 0, 0));
	void RenderFontInWorldCoordinate(const wchar_t* text, int textSize, double size, PERVec2 pos, PERColor color);

	// ��ũ�� ��ǥ�� ������(UI ��)
	void RenderShapeInScreenCoordinate(PERShapeType type, PERVec2 pos, PERVec2 size, PERColor color,
		bool border = true, int borderWidth = 1, PERColor borderColor = PERColor(0, 0, 0));
	void RenderFontInScreenCoordinate(const wchar_t* text, int textSize, double size, PERVec2 posWithRate, PERColor color);

private:
	void RenderShape(PERShapeType type, PERVec2 pos, PERVec2 size, PERColor color,
		bool border, int borderWidth, PERColor borderColor);
	void RenderFont(const wchar_t* text, int textSize, double size, PERVec2 pos, PERColor color);

	// ���� ��� ������
	void RenderEllipse(PERVec2 pos, PERVec2 size);
	void RenderRectangle(PERVec2 pos, PERVec2 size);
	void RenderRoundRectangle(PERVec2 pos, PERVec2 size, int widthAngle, int heightAngle);
	void RenderTriangle(PERVec2 pos, PERVec2 size);
	// ����, �� ��Ŀ ����Ʈ
	void RenderEllipseWithLeftTopAnchor(PERVec2 pos, PERVec2 size);
	void RenderRectangleWithLeftTopAnchor(PERVec2 pos, PERVec2 size);
	void RenderRoundRectangleWithLeftTopAnchor(PERVec2 pos, PERVec2 size, int widthAngle, int heightAngle);
	void RenderTriangleWithLeftTopAnchor(PERVec2 pos, PERVec2 size);

	// openGL�� �������� ��ǥ�谣 ��ȯ
	// ���� ��ǥ�迡�� ��ȯ(-ī�޶� �þ� ũŰ + ī�޶� ��ǥ ~ ī�޶� �þ� ũŰ + ī�޶� ��ǥ)
	PERVec2 ConvertWorldCoordinateOpenGLToWindows(PERVec2 pos);
	PERVec2 MatchSizeWithWorldCoordinate(PERVec2 size);
	double MatchSizeWithWorldCoordinate(double size);
	// ��ũ�� ��ǥ�迡�� ��ȯ(-1.0 ~ 1.0)
	PERVec2 ConvertScreenCoordinateOpenGLToWindows(PERVec2 pos);
	PERVec2 MatchSizeWithScreenCoordinate(PERVec2 size);
	double MatchSizeWithScreenCoordinate(double size);

	PERVec2 m_cameraPosition = PERVec2(0.0, 0.0);
	double m_cameraSightWidth = 20.0;

	double m_halfWidth;
	double m_halfHeight;

	HDC m_memoryDC;
};