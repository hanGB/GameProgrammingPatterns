#pragma once

enum class SlShapeType {
	SHAPE_TYPE_ELLIPSE,
	SHAPE_TYPE_RECTANGLE,
	SHAPE_TYPE_TRIANGLE
};

class SlRenderer {
public:
	SlRenderer();
	~SlRenderer();

	void SetWindowSizeAndNowFrameMemoryDC(HWND hWnd, HDC& memDC);
	void RenderShape(SlShapeType type, SlVector3<double> pos, SlVector2<double> size, SlColor color);
	void RenderFont(const wchar_t* text, int textSize, double size, SlVector2<double> pos, SlColor color);

private:
	void RenderEllipse(SlVector3<double> pos, SlVector2<double> size, SlColor color);
	void RenderRectangle(SlVector3<double> pos, SlVector2<double> size, SlColor color);
	void RenderTriangle(SlVector3<double> pos, SlVector2<double> size, SlColor color);

	void SetColor(SlColor color, HPEN& hPen, HPEN& oldPen, HBRUSH& hBrush, HBRUSH& oldBrush);
	void DeleteColor(HPEN& hPen, HPEN& oldPen, HBRUSH& hBrush, HBRUSH& oldBrush);

	void ConvertCoordinateOpenGLToWindows(double* x, double* y);
	void MatchToCurrentWindowSize(double* x, double* y);
	void MatchToCurrentWindowSizeForFont(double* size);

	HDC* m_memoryDC;
	double m_WindowHalfWidth;
	double m_WindowHalfHeight;
};