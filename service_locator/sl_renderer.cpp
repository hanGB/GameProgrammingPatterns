#include "stdafx.h"
#include "sl_renderer.h"

SlRenderer::SlRenderer()
{

}

SlRenderer::~SlRenderer()
{

}

void SlRenderer::SetWindowSizeAndNowFrameMemoryDC(HWND hWnd, HDC& memDC)
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	m_WindowHalfWidth = (double)rect.right / 2.0;
	m_WindowHalfHeight = (double)rect.bottom / 2.0;

	m_memoryDC = &memDC;
}

void SlRenderer::RenderShape(SlShapeType type, SlVector3<double> pos, SlVector2<double> size, SlColor color)
{
	ConvertCoordinateOpenGLToWindows(&pos.x, &pos.y);
	MatchToCurrentWindowSize(&size.x, &size.y);

	switch (type) {
	case SlShapeType::SHAPE_TYPE_ELLIPSE:
		RenderEllipse(pos, size, color);
		break;
	case SlShapeType::SHAPE_TYPE_RECTANGLE:
		RenderRectangle(pos, size, color);
		break;
	case SlShapeType::SHAPE_TYPE_TRIANGLE:
		RenderTriangle(pos, size, color);
		break;

	default:
		RenderTriangle(pos, size, color);
		break;
	}
}

void SlRenderer::RenderFont(const wchar_t* text, int textSize, double size, SlVector2<double> pos, SlColor color)
{
	HPEN hPen, oldPen;
	HFONT hFont, oldFont;

	hPen = (HPEN)GetStockObject(NULL_PEN);  // ≈ı∏Ìº±
	oldPen = (HPEN)SelectObject(*m_memoryDC, hPen);
	MatchToCurrentWindowSizeForFont(&size);
	hFont = CreateFont(static_cast<int>(size * SL_PIXEL_PER_METER), 0, 0, 0,
		FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"∏º¿∫ ∞ÌµÒ");
	SetTextColor(*m_memoryDC, RGB(color.r, color.g, color.b));
	SetBkMode(*m_memoryDC, TRANSPARENT);
	oldFont = (HFONT)SelectObject(*m_memoryDC, hFont);

	ConvertCoordinateOpenGLToWindows(&pos.x, &pos.y);

	TextOut(*m_memoryDC,
		static_cast<int>(pos.x * SL_PIXEL_PER_METER),
		static_cast<int>(pos.y * SL_PIXEL_PER_METER), text, textSize);

	SelectObject(*m_memoryDC, oldFont);
	DeleteObject(hFont);
	SelectObject(*m_memoryDC, oldPen);
	DeleteObject(hPen);
}

void SlRenderer::RenderEllipse(SlVector3<double> pos, SlVector2<double> size, SlColor color)
{
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;

	SetColor(color, hPen, oldPen, hBrush, oldBrush);

	double hSizeX = size.x / 2.0, hSizeY = size.y / 2.0;
	Ellipse(*m_memoryDC,
		static_cast<int>((pos.x - hSizeX) * SL_PIXEL_PER_METER),
		static_cast<int>((pos.y - hSizeY) * SL_PIXEL_PER_METER),
		static_cast<int>((pos.x + hSizeX) * SL_PIXEL_PER_METER),
		static_cast<int>((pos.y + hSizeY) * SL_PIXEL_PER_METER)
	);

	DeleteColor(hPen, oldPen, hBrush, oldBrush);
}

void SlRenderer::RenderRectangle(SlVector3<double> pos, SlVector2<double> size, SlColor color)
{
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;

	SetColor(color, hPen, oldPen, hBrush, oldBrush);

	double hSizeX = size.x / 2.0, hSizeY = size.y / 2.0;
	Rectangle(*m_memoryDC,
		static_cast<int>((pos.x - hSizeX) * SL_PIXEL_PER_METER),
		static_cast<int>((pos.y - hSizeY) * SL_PIXEL_PER_METER),
		static_cast<int>((pos.x + hSizeX) * SL_PIXEL_PER_METER),
		static_cast<int>((pos.y + hSizeY) * SL_PIXEL_PER_METER)
	);

	DeleteColor(hPen, oldPen, hBrush, oldBrush);
}

void SlRenderer::RenderTriangle(SlVector3<double> pos, SlVector2<double> size, SlColor color)
{
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;

	SetColor(color, hPen, oldPen, hBrush, oldBrush);

	double hSizeX = size.x / 2.0, hSizeY = size.y / 2.0;
	POINT vertices[] = {
		{ (long)pos.x,(long)(pos.y + hSizeY) },
		{ (long)(pos.x - hSizeX), (long)(pos.y - hSizeY) },
		{ (long)(pos.x + hSizeX), (long)(pos.y - hSizeY) }
	};
	Polygon(*m_memoryDC, vertices, sizeof(vertices) / sizeof(vertices[0]));

	DeleteColor(hPen, oldPen, hBrush, oldBrush);
}

void SlRenderer::SetColor(SlColor color, HPEN& hPen, HPEN& oldPen, HBRUSH& hBrush, HBRUSH& oldBrush)
{
	hPen = CreatePen(PS_SOLID, 0, RGB(color.r, color.g, color.b));
	hBrush = CreateSolidBrush(RGB(color.r, color.g, color.b));
	oldPen = (HPEN)SelectObject(*m_memoryDC, hPen);
	oldBrush = (HBRUSH)SelectObject(*m_memoryDC, hBrush);
}

void SlRenderer::DeleteColor(HPEN& hPen, HPEN& oldPen, HBRUSH& hBrush, HBRUSH& oldBrush)
{
	(HPEN)SelectObject(*m_memoryDC, oldPen);
	(HBRUSH)SelectObject(*m_memoryDC, oldBrush);

	DeleteObject(oldPen);
	DeleteObject(oldBrush);
	DeleteObject(hPen);
	DeleteObject(hBrush);
}

void SlRenderer::ConvertCoordinateOpenGLToWindows(double* x, double* y)
{
	*x += SL_BASE_WINDOW_WIDTH / (double)SL_PIXEL_PER_METER;
	*y *= -1;
	*y += SL_BASE_WINDOW_HEIGHT / (double)SL_PIXEL_PER_METER;

	MatchToCurrentWindowSize(x, y);
}

void SlRenderer::MatchToCurrentWindowSize(double* x, double* y)
{
	*x *= (m_WindowHalfWidth / (double)SL_BASE_WINDOW_WIDTH);
	*y *= (m_WindowHalfHeight / (double)SL_BASE_WINDOW_HEIGHT);
}

void SlRenderer::MatchToCurrentWindowSizeForFont(double* size)
{
	*size *= (m_WindowHalfWidth / (double)SL_BASE_WINDOW_WIDTH);
}

