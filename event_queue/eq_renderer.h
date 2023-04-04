#pragma once

struct CompCoordinateData {
	
};

enum class EqShapeType {
	EQ_SHAPE_TYPE_ELLIPSE,
	EQ_SHAPE_TYPE_RECTANGLE,
	EQ_SHAPE_TYPE_TRIANGLE
};

class EqRenderer {
public:
	EqRenderer();
	~EqRenderer();

	void SetWindowSizeAndNowFrameMemoryDC(HWND hWnd, HDC& memDC);
	void RenderShape(EqShapeType type, EqVector3<double> pos, EqVector2<double> size, EqColor color);
	void RenderFont(const wchar_t* text, int textSize, double size, EqVector2<double> pos, EqColor color);

private:
	void RenderEllipse(EqVector3<double> pos, EqVector2<double> size, EqColor color);
	void RenderRectangle(EqVector3<double> pos, EqVector2<double> size, EqColor color);
	void RenderTriangle(EqVector3<double> pos, EqVector2<double> size, EqColor color);

	void SetColor(EqColor color, HPEN& hPen, HPEN& oldPen, HBRUSH& hBrush, HBRUSH& oldBrush);
	void DeleteColor(HPEN& hPen, HPEN& oldPen, HBRUSH& hBrush, HBRUSH& oldBrush);

	void ConvertCoordinateOpenGLToWindows(double* x, double* y);
	void MatchToCurrentWindowSize(double* x, double* y);
	void MatchToCurrentWindowSizeForFont(double* size);

	HDC* m_memoryDC;
	double m_WindowHalfWidth;
	double m_WindowHalfHeight;
};