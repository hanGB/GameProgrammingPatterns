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

struct CompRGBColor {
	int red;
	int green;
	int blue;
};


class CompRenderer {
public:
	CompRenderer(double halfWidth, double halfHeight);
	~CompRenderer();

	void SetNowFrameMemoryDC(HDC& memDC);
	void RenderEllipse(double posX, double posY, double sizeX, double sizeY, CompRGBColor color);
	void RenderRectangle(double posX, double posY, double sizeX, double sizeY, CompRGBColor color);
	void RenderTriangle(double posX, double posY, double sizeX, double sizeY, CompRGBColor color);

private:
	void SetColor(CompRGBColor color, HPEN& hPen, HPEN& oldPen, HBRUSH& hBrush, HBRUSH& oldBrush);
	void DeleteColor(HPEN& hPen, HPEN& oldPen, HBRUSH& hBrush, HBRUSH& oldBrush);

	HDC* m_memoryDC;
	CompCoordinateData m_coordinateData;
};