#pragma once

struct CompCoordinateData {
	double halfWidth;
	double halfHeight;

	void ConvertCoordinateOpenGLToWindows(double* x, double* y) {
		*x += halfWidth / (double)COMP_PIXEL_PER_METER;
		*y *= -1;
		*y += halfHeight / (double)COMP_PIXEL_PER_METER;
	}
};

enum class CompShapeType {
	COMP_SHAPE_TYPE_ELLIPSE,
	COMP_SHAPE_TYPE_RECTANGLE,
	COMP_SHAPE_TYPE_TRIANGLE
};

class CompRenderer {
public:
	CompRenderer(double halfWidth, double halfHeight);
	~CompRenderer();

	void SetNowFrameMemoryDC(HDC& memDC);
	void RenderShape(CompShapeType type, CompVector3<double> pos, CompVector2<double> size, CompColor color);

private:
	void RenderEllipse(CompVector3<double> pos, CompVector2<double> size, CompColor color);
	void RenderRectangle(CompVector3<double> pos, CompVector2<double> size, CompColor color);
	void RenderTriangle(CompVector3<double> pos, CompVector2<double> size, CompColor color);

	void SetColor(CompColor color, HPEN& hPen, HPEN& oldPen, HBRUSH& hBrush, HBRUSH& oldBrush);
	void DeleteColor(HPEN& hPen, HPEN& oldPen, HBRUSH& hBrush, HBRUSH& oldBrush);

	HDC* m_memoryDC;
	CompCoordinateData m_coordinateData;
};