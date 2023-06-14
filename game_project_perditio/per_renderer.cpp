#include "stdafx.h"
#include "per_renderer.h"

PERRenderer::PERRenderer()
{
	m_halfWidth = PER_DEFAULT_WINDOW_WIDTH / 2;
	m_halfHeight = PER_DEFAULT_WINDOW_HEIGHT / 2;
}

PERRenderer::~PERRenderer()
{
}

void PERRenderer::MatchWindowSizeAndCurrentMemoryDC(HWND hWnd, HDC memDC)
{
	RECT rect;
	GetClientRect(hWnd, &rect);

	m_halfWidth = rect.right / 2.0;
	m_halfHeight = rect.bottom / 2.0;

	m_memoryDC = memDC;
}

void PERRenderer::RenderWorld(PERWorld& world)
{
	PERVec3 pos(0.0, 0.0, 0.0);
	PERVec2 size(1.0, 1.0);
	PERColor color(255, 255, 255);

	RenderShape(PERShapeType::SHAPE_TYPE_RECTANGLE, pos, size, color);
}

void PERRenderer::RenderShape(PERShapeType type, PERVec3 pos, PERVec2 size, PERColor color)
{
	HBRUSH newBrush, oldBrush;
	newBrush = CreateSolidBrush(RGB(color.r, color.g, color.b));
	oldBrush = (HBRUSH)SelectObject(m_memoryDC, newBrush);

	// 위치 좌표 변환, 크기 화면에 맞추어 변경
	pos = ConvertCoordinateOpenGLToWindowsForVec3(pos);
	pos = pos * PER_PIXEL_PER_METER;
	size = MatchSizeWithScreenSizeAndRatioForVec2(size);
	size = size * PER_PIXEL_PER_METER;

	switch (type) {
	case PERShapeType::SHAPE_TYPE_ELLIPSE:
		RenderEllipse(pos, size);
		break;
	case PERShapeType::SHAPE_TYPE_RECTANGLE:
		RenderRectangle(pos, size);
		break;
	case PERShapeType::SHAPE_TYPE_TRIANGLE:
		RenderTriangle(pos, size);
		break;
	}

	(HBRUSH)SelectObject(m_memoryDC, oldBrush);
	DeleteObject(newBrush);
	DeleteObject(oldBrush);
}

void PERRenderer::RenderEllipse(PERVec3 pos, PERVec2 size)
{
	Ellipse(m_memoryDC, (int)(pos.x - size.x), (int)(pos.y - size.y), (int)(pos.x + size.x), (int)(pos.y + size.y));
}

void PERRenderer::RenderRectangle(PERVec3 pos, PERVec2 size)
{
	Rectangle(m_memoryDC, (int)(pos.x - size.x), (int)(pos.y - size.y), (int)(pos.x + size.x), (int)(pos.y + size.y));
}

void PERRenderer::RenderTriangle(PERVec3 pos, PERVec2 size)
{
	POINT vertices[] = {
		{ (long)pos.x,				  (long)(pos.y + size.y / 2) },
		{ (long)(pos.x - size.x / 2), (long)(pos.y - size.y / 2) },
		{ (long)(pos.x + size.x / 2), (long)(pos.y - size.y / 2) }
	};
	Polygon(m_memoryDC, vertices, sizeof(vertices) / sizeof(vertices[0]));
}


void PERRenderer::ConvertCoordinateOpenGLToWindows(double* x, double* y)
{
	*x += m_halfWidth / (double)PER_PIXEL_PER_METER;
	*y *= -1;
	*y += m_halfHeight / (double)PER_PIXEL_PER_METER;
}
void PERRenderer::ConvertCoordinateWindowsToOpenGL(double* x, double* y)
{
	*x -= m_halfWidth;
	*y -= m_halfHeight;
	*y *= -1;

	*x /= (double)PER_PIXEL_PER_METER;
	*y /= (double)PER_PIXEL_PER_METER;
}

PERVec3 PERRenderer::ConvertCoordinateOpenGLToWindowsForVec3(PERVec3 vec)
{
	vec.x += m_halfWidth / (double)PER_PIXEL_PER_METER;
	vec.y *= -1;
	vec.y += m_halfHeight / (double)PER_PIXEL_PER_METER;

	return vec;
}

PERVec2 PERRenderer::ConvertCoordinateOpenGLToWindowsForVec2(PERVec2 vec)
{
	vec.x += m_halfWidth / (double)PER_PIXEL_PER_METER;
	vec.y *= -1;
	vec.y += m_halfHeight / (double)PER_PIXEL_PER_METER;

	return vec;
}

void PERRenderer::MatchSizeWithScreenSizeAndRatio(double* x, double* y)
{
	*x *= m_halfWidth / (double)PER_DEFAULT_WINDOW_WIDTH;
	*y *= m_halfHeight / (double)PER_DEFAULT_WINDOW_HEIGHT;
}

PERVec3 PERRenderer::MatchSizeWithScreenSizeAndRatioForVec3(PERVec3 vec)
{
	vec.x *= m_halfWidth / (double)PER_DEFAULT_WINDOW_WIDTH;
	vec.y *= m_halfHeight / (double)PER_DEFAULT_WINDOW_HEIGHT;

	return vec;
}

PERVec2 PERRenderer::MatchSizeWithScreenSizeAndRatioForVec2(PERVec2 vec)
{
	vec.x *= m_halfWidth / (double)PER_DEFAULT_WINDOW_WIDTH;
	vec.y *= m_halfHeight / (double)PER_DEFAULT_WINDOW_HEIGHT;

	return vec;
}