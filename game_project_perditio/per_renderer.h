#pragma once

class PERRenderer {
public:
	PERRenderer();
	~PERRenderer();

	// ���� ������ ������� �޸�DC ����
	void MatchWindowSizeAndCurrentMemoryDC(HWND hWnd, HDC memDC);
	// ��� ������
	void RenderShape(PERShapeType type, PERVec3 pos, PERVec3 size, PERColor color);
	void RenderFont(const wchar_t* text, int textSize, double size, PERVec2 pos, PERColor color);

private:
	// ���� ��� ������
	void RenderEllipse(PERVec3 pos, PERVec3 size);
	void RenderRectangle(PERVec3 pos, PERVec3 size);
	void RenderTriangle(PERVec3 pos, PERVec3 size);

	// openGL�� �������� ��ǥ�谣 ��ȯ
	void ConvertCoordinateOpenGLToWindows(double* x, double* y);
	void ConvertCoordinateWindowsToOpenGL(double* x, double* y);
	PERVec3 ConvertCoordinateOpenGLToWindowsForVec3(PERVec3 vec);
	PERVec2 ConvertCoordinateOpenGLToWindowsForVec2(PERVec2 vec);

	// ȭ�� ũ��� ������ ���߾� ������ ��ȯ
	void MatchSizeWithScreenSizeAndRatio(double* x, double* y);
	PERVec3 MatchSizeWithScreenSizeAndRatioForVec3(PERVec3 vec);
	PERVec2 MatchSizeWithScreenSizeAndRatioForVec2(PERVec2 vec);
	double MatchSizeWithScreenSizeAndRatioForFont(double size);

	double m_halfWidth;
	double m_halfHeight;

	HDC m_memoryDC;
};