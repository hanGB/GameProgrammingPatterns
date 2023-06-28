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

void PERRenderer::SetCameraPosition(PERVec2 pos)
{
	m_cameraPosition = pos;
}

void PERRenderer::SetCameraSightWidth(double width)
{
	m_cameraSightWidth = width;
}

void PERRenderer::RenderShapeInWorldCoordinate(PERShapeType type, PERVec3 pos, PERVec3 size, PERColor color, 
	bool border, int borderWidth, PERColor borderColor)
{
	// 위치 좌표 변환, 크기 화면에 맞추어 변경
	PERVec2 posInVec2 = ConvertWorldCoordinateOpenGLToWindows(PERVec2(pos.x, pos.y));
	PERVec2 sizeInVec2 = MatchSizeWithWorldCoordinate(PERVec2(size.x, size.y));

	RenderShape(type, posInVec2, sizeInVec2, color, border, borderWidth, borderColor);
}

void PERRenderer::RenderFontInWorldCoordinate(const wchar_t* text, int textSize, double size, PERVec2 pos, PERColor color)
{
	pos = ConvertWorldCoordinateOpenGLToWindows(pos);
	size = MatchSizeWithWorldCoordinate(size);

	RenderFont(text, textSize, size, pos, color);
}

void PERRenderer::RenderShapeInScreenCoordinate(PERShapeType type, PERVec2 pos, PERVec2 size, PERColor color, 
	bool border, int borderWidth, PERColor borderColor)
{
	pos = ConvertScreenCoordinateOpenGLToWindows(pos);
	size = MatchSizeWithScreenCoordinate(size);

	RenderShape(type, pos, size, color, border, borderWidth, borderColor);
}

void PERRenderer::RenderFontInScreenCoordinate(const wchar_t* text, int textSize, double size, PERVec2 pos, PERColor color)
{
	pos = ConvertScreenCoordinateOpenGLToWindows(pos);
	size = MatchSizeWithScreenCoordinate(size);

	RenderFont(text, textSize, size, pos, color);
}

void PERRenderer::RenderShape(PERShapeType type, PERVec2 pos, PERVec2 size, PERColor color,
	bool border, int borderWidth, PERColor borderColor)
{
	HBRUSH newBrush, oldBrush;
	newBrush = CreateSolidBrush(RGB(color.r, color.g, color.b));
	oldBrush = (HBRUSH)SelectObject(m_memoryDC, newBrush);

	HPEN newPen, oldPen;
	if (border) newPen = CreatePen(PS_SOLID, borderWidth, RGB(borderColor.r, borderColor.g, borderColor.b));
	else newPen = CreatePen(PS_SOLID, 0, RGB(color.r, color.g, color.b));
	oldPen = (HPEN)SelectObject(m_memoryDC, newPen);

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
	newFont = CreateFont((int)size, 0, 0, 0,
		FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"맑은 고딕");
	SetTextColor(m_memoryDC, RGB(color.r, color.g, color.b));
	SetBkMode(m_memoryDC, TRANSPARENT);
	oldFont = (HFONT)SelectObject(m_memoryDC, newFont);

	TextOut(m_memoryDC, (int)pos.x, (int)pos.y, text, textSize);

	SelectObject(m_memoryDC, oldFont);
	DeleteObject(newPen);
	DeleteObject(oldFont);
	SelectObject(m_memoryDC, oldPen);
	DeleteObject(newFont);
	DeleteObject(oldPen);
}

void PERRenderer::RenderEllipse(PERVec2 pos, PERVec2 size)
{
	Ellipse(m_memoryDC, (int)(pos.x - size.x / 2), (int)(pos.y - size.y / 2), (int)(pos.x + size.x / 2), (int)(pos.y + size.y / 2));
}

void PERRenderer::RenderRectangle(PERVec2 pos, PERVec2 size)
{
	Rectangle(m_memoryDC, (int)(pos.x - size.x / 2), (int)(pos.y - size.y / 2), (int)(pos.x + size.x / 2), (int)(pos.y + size.y / 2));
}

void PERRenderer::RenderRoundRectangle(PERVec2 pos, PERVec2 size, int widthAngle, int heightAngle)
{
	RoundRect(m_memoryDC, (int)(pos.x - size.x / 2), (int)(pos.y - size.y / 2), 
		(int)(pos.x + size.x / 2), (int)(pos.y + size.y / 2), widthAngle, heightAngle);
}

void PERRenderer::RenderTriangle(PERVec2 pos, PERVec2 size)
{
	POINT vertices[] = {
		{ (long)pos.x,				  (long)(pos.y - size.y / 2) },
		{ (long)(pos.x - size.x / 2), (long)(pos.y + size.y / 2) },
		{ (long)(pos.x + size.x / 2), (long)(pos.y + size.y / 2) }
	};
	Polygon(m_memoryDC, vertices, sizeof(vertices) / sizeof(vertices[0]));
}

void PERRenderer::RenderEllipseWithLeftTopAnchor(PERVec2 pos, PERVec2 size)
{
	Ellipse(m_memoryDC, (int)(pos.x), (int)(pos.y), (int)(pos.x + size.x), (int)(pos.y + size.y));
}

void PERRenderer::RenderRectangleWithLeftTopAnchor(PERVec2 pos, PERVec2 size)
{
	Rectangle(m_memoryDC, (int)(pos.x), (int)(pos.y), (int)(pos.x + size.x), (int)(pos.y + size.y));
}

void PERRenderer::RenderRoundRectangleWithLeftTopAnchor(PERVec2 pos, PERVec2 size, int widthAngle, int heightAngle)
{
	RoundRect(m_memoryDC, (int)(pos.x), (int)(pos.y), 
		(int)(pos.x + size.x), (int)(pos.y + size.y), widthAngle, heightAngle);
}

void PERRenderer::RenderTriangleWithLeftTopAnchor(PERVec2 pos, PERVec2 size)
{
	POINT vertices[] = {
		{ (long)(pos.x + size.x / 2),	(long)(pos.y - size.y) },
		{ (long)(pos.x),				(long)(pos.y) },
		{ (long)(pos.x + size.x),		(long)(pos.y) }
	};
	Polygon(m_memoryDC, vertices, sizeof(vertices) / sizeof(vertices[0]));
}

PERVec2 PERRenderer::ConvertWorldCoordinateOpenGLToWindows(PERVec2 pos)
{
	pos.y *= -1;

	double cameraSightHeight = m_cameraSightWidth * m_halfHeight / m_halfWidth;

	double widthSizePerMeter = (m_halfWidth * 2.0) / m_cameraSightWidth;
	double heightSizePerMeter = (m_halfHeight * 2.0) / cameraSightHeight;

	pos.x -= m_cameraPosition.x;
	pos.x += m_cameraSightWidth / 2.0;
	pos.x *= widthSizePerMeter;
	pos.y -= m_cameraPosition.y;
	pos.y += cameraSightHeight / 2.0;
	pos.y *= heightSizePerMeter;

	return pos;
}

PERVec2 PERRenderer::MatchSizeWithWorldCoordinate(PERVec2 size)
{
	double cameraSightHeight = m_cameraSightWidth * m_halfHeight / m_halfWidth;

	double widthSizePerMeter = (m_halfWidth * 2.0) / m_cameraSightWidth;
	double heightSizePerMeter = (m_halfHeight * 2.0) / cameraSightHeight;

	size.x *= widthSizePerMeter;
	size.y *= heightSizePerMeter;

	return size;
}

double PERRenderer::MatchSizeWithWorldCoordinate(double size)
{
	double cameraSightHeight = m_cameraSightWidth * m_halfHeight / m_halfWidth;

	double width = (m_halfWidth * 2.0) / m_cameraSightWidth;
	double height = (m_halfHeight * 2.0) / cameraSightHeight;

	size *= (width + height) / 2.0;

	return size;
}

PERVec2 PERRenderer::ConvertScreenCoordinateOpenGLToWindows(PERVec2 pos)
{
	pos.y *= -1;

	pos.x += 1.0;
	pos.x *= m_halfWidth;
	pos.y += 1.0;
	pos.y *= m_halfHeight;

	return pos;
}

PERVec2 PERRenderer::MatchSizeWithScreenCoordinate(PERVec2 pos)
{
	pos.x *= m_halfWidth;
	pos.y *= m_halfHeight;

	return pos;
}

double PERRenderer::MatchSizeWithScreenCoordinate(double size)
{
	size *= (m_halfWidth + m_halfHeight) / 2.0;

	return size;
}
