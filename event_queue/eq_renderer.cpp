#include "stdafx.h"
#include "eq_renderer.h"

EqRenderer::EqRenderer()
{

}

EqRenderer::~EqRenderer()
{

}

void EqRenderer::SetWindowSizeAndNowFrameMemoryDC(HWND hWnd, HDC& memDC)
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	m_WindowHalfWidth = (double)rect.right / 2.0;
	m_WindowHalfHeight = (double)rect.bottom / 2.0;

	m_memoryDC = &memDC;
}

void EqRenderer::RenderShape(EqShapeType type, EqVector3<double> pos, EqVector2<double> size, EqColor color)
{
	ConvertCoordinateOpenGLToWindows(&pos.x, &pos.y);
	MatchToCurrentWindowSize(&size.x, &size.y);

	switch (type) {
	case EqShapeType::EQ_SHAPE_TYPE_ELLIPSE:
		RenderEllipse(pos, size, color);
		break;
	case EqShapeType::EQ_SHAPE_TYPE_RECTANGLE:
		RenderRectangle(pos, size, color);
		break;
	case EqShapeType::EQ_SHAPE_TYPE_TRIANGLE:
		RenderTriangle(pos, size, color);
		break;

	default:
		RenderTriangle(pos, size, color);
		break;
	}
}

void EqRenderer::RenderFont(const wchar_t* text, int textSize, double size, EqVector2<double> pos, EqColor color)
{
	HPEN hPen, oldPen;
	HFONT hFont, oldFont;

	hPen = (HPEN)GetStockObject(NULL_PEN);  // ≈ı∏Ìº±
	oldPen = (HPEN)SelectObject(*m_memoryDC, hPen);
	MatchToCurrentWindowSizeForFont(&size);
	hFont = CreateFont(static_cast<int>(size * EQ_PIXEL_PER_METER), 0, 0, 0, 
		FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"∏º¿∫ ∞ÌµÒ");
	SetTextColor(*m_memoryDC, RGB(color.r, color.g, color.b));
	SetBkMode(*m_memoryDC, TRANSPARENT);
	oldFont = (HFONT)SelectObject(*m_memoryDC, hFont);

	ConvertCoordinateOpenGLToWindows(&pos.x, &pos.y);

	TextOut(*m_memoryDC, 
		static_cast<int>(pos.x * EQ_PIXEL_PER_METER), 
		static_cast<int>(pos.y * EQ_PIXEL_PER_METER), text, textSize);

	SelectObject(*m_memoryDC, oldFont);
	DeleteObject(hFont);
	SelectObject(*m_memoryDC, oldPen);
	DeleteObject(hPen);
}

void EqRenderer::RenderEllipse(EqVector3<double> pos, EqVector2<double> size, EqColor color)
{
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;

	SetColor(color, hPen, oldPen, hBrush, oldBrush);

	double hSizeX = size.x / 2.0, hSizeY = size.y / 2.0;
	Ellipse(*m_memoryDC,
		static_cast<int>((pos.x - hSizeX) * EQ_PIXEL_PER_METER),
		static_cast<int>((pos.y - hSizeY) * EQ_PIXEL_PER_METER),
		static_cast<int>((pos.x + hSizeX) * EQ_PIXEL_PER_METER),
		static_cast<int>((pos.y + hSizeY) * EQ_PIXEL_PER_METER)
	);

	DeleteColor(hPen, oldPen, hBrush, oldBrush);
}

void EqRenderer::RenderRectangle(EqVector3<double> pos, EqVector2<double> size, EqColor color)
{
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;

	SetColor(color, hPen, oldPen, hBrush, oldBrush);

	double hSizeX = size.x / 2.0, hSizeY = size.y / 2.0;
	Rectangle(*m_memoryDC,
		static_cast<int>((pos.x - hSizeX) * EQ_PIXEL_PER_METER),
		static_cast<int>((pos.y - hSizeY) * EQ_PIXEL_PER_METER),
		static_cast<int>((pos.x + hSizeX) * EQ_PIXEL_PER_METER),
		static_cast<int>((pos.y + hSizeY) * EQ_PIXEL_PER_METER)
	);

	DeleteColor(hPen, oldPen, hBrush, oldBrush);
}

void EqRenderer::RenderTriangle(EqVector3<double> pos, EqVector2<double> size, EqColor color)
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

void EqRenderer::SetColor(EqColor color, HPEN& hPen, HPEN& oldPen, HBRUSH& hBrush, HBRUSH& oldBrush)
{
	hPen = CreatePen(PS_SOLID, 0, RGB(color.r, color.g, color.b));
	hBrush = CreateSolidBrush(RGB(color.r, color.g, color.b));
	oldPen = (HPEN)SelectObject(*m_memoryDC, hPen);
	oldBrush = (HBRUSH)SelectObject(*m_memoryDC, hBrush);
}

void EqRenderer::DeleteColor(HPEN& hPen, HPEN& oldPen, HBRUSH& hBrush, HBRUSH& oldBrush)
{
	(HPEN)SelectObject(*m_memoryDC, oldPen);
	(HBRUSH)SelectObject(*m_memoryDC, oldBrush);

	DeleteObject(oldPen);
	DeleteObject(oldBrush);
	DeleteObject(hPen);
	DeleteObject(hBrush);
}

void EqRenderer::ConvertCoordinateOpenGLToWindows(double* x, double* y)
{
	*x += EQ_BASE_WINDOW_WIDTH / (double)EQ_PIXEL_PER_METER;
	*y *= -1;
	*y += EQ_BASE_WINDOW_HEIGHT / (double)EQ_PIXEL_PER_METER;

	MatchToCurrentWindowSize(x, y);
}

void EqRenderer::MatchToCurrentWindowSize(double* x, double* y)
{
	*x *= (m_WindowHalfWidth / (double)EQ_BASE_WINDOW_WIDTH);
	*y *= (m_WindowHalfHeight / (double)EQ_BASE_WINDOW_HEIGHT);
}

void EqRenderer::MatchToCurrentWindowSizeForFont(double* size)
{
	*size *= (m_WindowHalfWidth / (double)EQ_BASE_WINDOW_WIDTH);
}

