#include "stdafx.h"
#include "eq_renderer.h"

EqRenderer::EqRenderer(double halfWidth, double halfHeight)
{
	m_WindowHalfWidth = halfWidth;
	m_WindowHalfHeight = halfHeight;
}

EqRenderer::~EqRenderer()
{

}

void EqRenderer::SetNowFrameMemoryDC(HDC& memDC)
{
	m_memoryDC = &memDC;
}

void EqRenderer::RenderShape(EqShapeType type, EqVector3<double> pos, EqVector2<double> size, EqColor color)
{
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

void EqRenderer::RenderEllipse(EqVector3<double> pos, EqVector2<double> size, EqColor color)
{
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;

	SetColor(color, hPen, oldPen, hBrush, oldBrush);

	ConvertCoordinateOpenGLToWindows(&pos.x, &pos.y);
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

	ConvertCoordinateOpenGLToWindows(&pos.x, &pos.y);
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

	ConvertCoordinateOpenGLToWindows(&pos.x, &pos.y);
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
	*x += m_WindowHalfWidth / (double)EQ_PIXEL_PER_METER;
	*y *= -1;
	*y += m_WindowHalfHeight / (double)EQ_PIXEL_PER_METER;
}

