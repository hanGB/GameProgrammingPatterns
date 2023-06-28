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

void PERRenderer::RenderShape(PERShapeType type, PERVec3 pos, PERVec3 size, PERColor color, 
	bool border, int borderWidth, PERColor borderColor)
{
	HBRUSH newBrush, oldBrush;
	newBrush = CreateSolidBrush(RGB(color.r, color.g, color.b));
	oldBrush = (HBRUSH)SelectObject(m_memoryDC, newBrush);

	HPEN newPen, oldPen;
	if (border) newPen = CreatePen(PS_SOLID, borderWidth, RGB(borderColor.r, borderColor.g, borderColor.b));
	else newPen = CreatePen(PS_SOLID, 0, RGB(color.r, color.g, color.b));
	oldPen = (HPEN)SelectObject(m_memoryDC, newPen);

	// 위치 좌표 변환, 크기 화면에 맞추어 변경
	pos = ConvertCoordinateOpenGLToWindowsForVec3(pos);
	pos = pos * PER_PIXEL_PER_METER;
	size = MatchSizeWithScreenSizeAndRatioForVec3(size);
	size = size * PER_PIXEL_PER_METER;

	switch (type) {
	case PERShapeType::SHAPE_TYPE_ELLIPSE:
		RenderEllipse(pos, size);
		break;
	case PERShapeType::SHAPE_TYPE_RECTANGLE:
		RenderRectangle(pos, size);
		break;
	case PERShapeType::SHAPE_TYPE_ROUND_RECTANGLE:
		RenderRoundRectangle(pos, size, 20, 20);
		break;
	case PERShapeType::SHAPE_TYPE_TRIANGLE:
		RenderTriangle(pos, size);
		break;

	// 왼쪽 위 앵커 용
	case PERShapeType::SHAPE_TYPE_ELLIPSE_WITH_LEFT_TOP_ANCHOR:
		RenderEllipseWithLeftTopAnchor(pos, size);
		break;
	case PERShapeType::SHAPE_TYPE_RECTANGLE_WITH_LEFT_TOP_ANCHOR:
		RenderRectangleWithLeftTopAnchor(pos, size);
		break;
	case PERShapeType::SHAPE_TYPE_ROUND_RECTANGLE_WITH_LEFT_TOP_ANCHOR:
		RenderRoundRectangleWithLeftTopAnchor(pos, size, 20, 20);
		break;
	case PERShapeType::SHAPE_TYPE_TRIANGLE_WITH_LEFT_TOP_ANCHOR:
		RenderTriangleWithLeftTopAnchor(pos, size);
		break;
	}

	SelectObject(m_memoryDC, oldBrush);
	DeleteObject(newBrush);
	DeleteObject(oldBrush);

	SelectObject(m_memoryDC, oldPen);
	DeleteObject(newPen);
	DeleteObject(oldPen);
}

void PERRenderer::RenderFont(const wchar_t* text, int textSize, double size, PERVec2 pos, PERColor color)
{
	HPEN newPen, oldPen;
	HFONT newFont, oldFont;

	newPen = (HPEN)GetStockObject(NULL_PEN);  // 투명선
	oldPen = (HPEN)SelectObject(m_memoryDC, newPen);
	size = MatchSizeWithScreenSizeAndRatioForFont(size);
	size = size * PER_PIXEL_PER_METER;
	newFont = CreateFont((int)size, 0, 0, 0,
		FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"맑은 고딕");
	SetTextColor(m_memoryDC, RGB(color.r, color.g, color.b));
	SetBkMode(m_memoryDC, TRANSPARENT);
	oldFont = (HFONT)SelectObject(m_memoryDC, newFont);

	pos = ConvertCoordinateOpenGLToWindowsForVec2(pos);
	pos = pos * PER_PIXEL_PER_METER;

	TextOut(m_memoryDC, (int)pos.x, (int)pos.y, text, textSize);

	SelectObject(m_memoryDC, oldFont);
	DeleteObject(newPen);
	DeleteObject(oldFont);
	SelectObject(m_memoryDC, oldPen);
	DeleteObject(newFont);
	DeleteObject(oldPen);
}

void PERRenderer::RenderEllipse(PERVec3 pos, PERVec3 size)
{
	Ellipse(m_memoryDC, (int)(pos.x - size.x), (int)(pos.y - size.y), (int)(pos.x + size.x), (int)(pos.y + size.y));
}

void PERRenderer::RenderRectangle(PERVec3 pos, PERVec3 size)
{
	Rectangle(m_memoryDC, (int)(pos.x - size.x), (int)(pos.y - size.y), (int)(pos.x + size.x), (int)(pos.y + size.y));
}

void PERRenderer::RenderRoundRectangle(PERVec3 pos, PERVec3 size, int widthAngle, int heightAngle)
{
	RoundRect(m_memoryDC, (int)(pos.x - size.x), (int)(pos.y - size.y), (int)(pos.x + size.x), (int)(pos.y + size.y), widthAngle, heightAngle);
}

void PERRenderer::RenderTriangle(PERVec3 pos, PERVec3 size)
{
	POINT vertices[] = {
		{ (long)pos.x,				  (long)(pos.y - size.y / 2) },
		{ (long)(pos.x - size.x / 2), (long)(pos.y + size.y / 2) },
		{ (long)(pos.x + size.x / 2), (long)(pos.y + size.y / 2) }
	};
	Polygon(m_memoryDC, vertices, sizeof(vertices) / sizeof(vertices[0]));
}

void PERRenderer::RenderEllipseWithLeftTopAnchor(PERVec3 pos, PERVec3 size)
{
	Ellipse(m_memoryDC, (int)(pos.x), (int)(pos.y), (int)(pos.x + size.x * 2), (int)(pos.y + size.y * 2));
}

void PERRenderer::RenderRectangleWithLeftTopAnchor(PERVec3 pos, PERVec3 size)
{
	Rectangle(m_memoryDC, (int)(pos.x), (int)(pos.y), (int)(pos.x + size.x * 2), (int)(pos.y + size.y * 2));
}

void PERRenderer::RenderRoundRectangleWithLeftTopAnchor(PERVec3 pos, PERVec3 size, int widthAngle, int heightAngle)
{
	RoundRect(m_memoryDC, (int)(pos.x), (int)(pos.y), (int)(pos.x + size.x * 2), (int)(pos.y + size.y * 2), widthAngle, heightAngle);
}

void PERRenderer::RenderTriangleWithLeftTopAnchor(PERVec3 pos, PERVec3 size)
{
	POINT vertices[] = {
		{ (long)(pos.x + size.x / 2),	(long)(pos.y - size.y) },
		{ (long)(pos.x),				(long)(pos.y) },
		{ (long)(pos.x + size.x),		(long)(pos.y) }
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

double PERRenderer::MatchSizeWithScreenSizeAndRatioForFont(double size)
{
	size *= m_halfWidth / (double)PER_DEFAULT_WINDOW_WIDTH;

	return size;
}
