#include "stdafx.h"
#include "comp_renderer.h"

CompRenderer::CompRenderer(double halfWidth, double halfHeight)
{
	m_coordinateData.halfWidth = halfWidth;
	m_coordinateData.halfHeight = halfHeight;
}

CompRenderer::~CompRenderer()
{

}

void CompRenderer::SetNowFrameMemoryDC(HDC& memDC)
{
	m_memoryDC = &memDC;
}

void CompRenderer::RenderEllipse(CompVector3<double> pos, CompVector2<double> size, CompColor color)
{
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;

	SetColor(color, hPen, oldPen, hBrush, oldBrush);

	m_coordinateData.ConvertCoordinateOpenGLToWindows(&pos.x, &pos.y);
	double hSizeX = size.x / 2.0, hSizeY = size.y / 2.0;

	Ellipse(*m_memoryDC,
		static_cast<int>((pos.x - hSizeX) * COMP_PIXEL_PER_METER),
		static_cast<int>((pos.y - hSizeY) * COMP_PIXEL_PER_METER),
		static_cast<int>((pos.x + hSizeX) * COMP_PIXEL_PER_METER),
		static_cast<int>((pos.y + hSizeY) * COMP_PIXEL_PER_METER)
	);

	DeleteColor(hPen, oldPen, hBrush, oldBrush);
}

void CompRenderer::RenderRectangle(CompVector3<double> pos, CompVector2<double> size, CompColor color)
{
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;

	SetColor(color, hPen, oldPen, hBrush, oldBrush);

	m_coordinateData.ConvertCoordinateOpenGLToWindows(&pos.x, &pos.y);
	double hSizeX = size.x / 2.0, hSizeY = size.y / 2.0;

	Rectangle(*m_memoryDC,
		static_cast<int>((pos.x - hSizeX) * COMP_PIXEL_PER_METER),
		static_cast<int>((pos.y - hSizeY) * COMP_PIXEL_PER_METER),
		static_cast<int>((pos.x + hSizeX) * COMP_PIXEL_PER_METER),
		static_cast<int>((pos.y + hSizeY) * COMP_PIXEL_PER_METER)
	);

	DeleteColor(hPen, oldPen, hBrush, oldBrush);
}

void CompRenderer::RenderTriangle(CompVector3<double> pos, CompVector2<double> size, CompColor color)
{
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;

	SetColor(color, hPen, oldPen, hBrush, oldBrush);

	m_coordinateData.ConvertCoordinateOpenGLToWindows(&pos.x, &pos.y);
	double hSizeX = size.x / 2.0, hSizeY = size.y / 2.0;

	POINT vertices[] = { 
		{ (long)pos.x,(long)(pos.y + hSizeY) },
		{ (long)(pos.x - hSizeX), (long)(pos.y - hSizeY) },
		{ (long)(pos.x + hSizeX), (long)(pos.y - hSizeY) }
	};
	Polygon(*m_memoryDC, vertices, sizeof(vertices) / sizeof(vertices[0]));

	DeleteColor(hPen, oldPen, hBrush, oldBrush);
}

void CompRenderer::SetColor(CompColor color, HPEN& hPen, HPEN& oldPen, HBRUSH& hBrush, HBRUSH& oldBrush)
{
	hPen = CreatePen(PS_SOLID, 0, RGB(color.r, color.g, color.b));
	hBrush = CreateSolidBrush(RGB(color.r, color.g, color.b));
	oldPen = (HPEN)SelectObject(*m_memoryDC, hPen);
	oldBrush = (HBRUSH)SelectObject(*m_memoryDC, hBrush);
}

void CompRenderer::DeleteColor(HPEN& hPen, HPEN& oldPen, HBRUSH& hBrush, HBRUSH& oldBrush)
{
	(HPEN)SelectObject(*m_memoryDC, oldPen);
	(HBRUSH)SelectObject(*m_memoryDC, oldBrush);

	DeleteObject(oldPen);
	DeleteObject(oldBrush);
	DeleteObject(hPen);
	DeleteObject(hBrush);
}
