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

void CompRenderer::RenderEllipse(double posX, double posY, double sizeX, double sizeY, CompRGBColor color)
{
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;

	SetColor(color, hPen, oldPen, hBrush, oldBrush);

	m_coordinateData.ConvertCoordinateOpenGLToWindows(&posX, &posY);
	double hSizeX = sizeX / 2.0, hSizeY = sizeY / 2.0;

	Ellipse(*m_memoryDC,
		static_cast<int>((posX - hSizeX) * COMP_PIXEL_PER_METER),
		static_cast<int>((posY - hSizeY) * COMP_PIXEL_PER_METER),
		static_cast<int>((posX + hSizeX) * COMP_PIXEL_PER_METER),
		static_cast<int>((posY + hSizeY) * COMP_PIXEL_PER_METER)
	);

	DeleteColor(hPen, oldPen, hBrush, oldBrush);
}

void CompRenderer::RenderRectangle(double posX, double posY, double sizeX, double sizeY, CompRGBColor color)
{
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;

	SetColor(color, hPen, oldPen, hBrush, oldBrush);

	m_coordinateData.ConvertCoordinateOpenGLToWindows(&posX, &posY);
	double hSizeX = sizeX / 2.0, hSizeY = sizeY / 2.0;

	Rectangle(*m_memoryDC,
		static_cast<int>((posX - hSizeX) * COMP_PIXEL_PER_METER),
		static_cast<int>((posY - hSizeY) * COMP_PIXEL_PER_METER),
		static_cast<int>((posX + hSizeX) * COMP_PIXEL_PER_METER),
		static_cast<int>((posY + hSizeY) * COMP_PIXEL_PER_METER)
	);

	DeleteColor(hPen, oldPen, hBrush, oldBrush);
}

void CompRenderer::RenderTriangle(double posX, double posY, double sizeX, double sizeY, CompRGBColor color)
{
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;

	SetColor(color, hPen, oldPen, hBrush, oldBrush);

	m_coordinateData.ConvertCoordinateOpenGLToWindows(&posX, &posY);
	double hSizeX = sizeX / 2.0, hSizeY = sizeY / 2.0;

	POINT vertices[] = { 
		{ (long)posX,(long)(posY + hSizeY) },
		{ (long)(posX - hSizeX), (long)(posY - hSizeY) },
		{ (long)(posX + hSizeX), (long)(posY - hSizeY) }
	};
	Polygon(*m_memoryDC, vertices, sizeof(vertices) / sizeof(vertices[0]));

	DeleteColor(hPen, oldPen, hBrush, oldBrush);
}

void CompRenderer::SetColor(CompRGBColor color, HPEN& hPen, HPEN& oldPen, HBRUSH& hBrush, HBRUSH& oldBrush)
{
	hPen = CreatePen(PS_SOLID, 0, RGB(color.red, color.green, color.blue));
	hBrush = CreateSolidBrush(RGB(color.red, color.green, color.blue));
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
