#include "stdafx.h"
#include "per_renderer.h"

PERRenderer::PERRenderer(HWND hWnd)
{
	PERLog::Logger().Info("렌더러 생성");

	m_windowSizeRect.left = 0;
	m_windowSizeRect.right = PER_DEFAULT_WINDOW_WIDTH - PER_DEFAULT_WINDOW_BORDER_WIDTH;
	m_windowSizeRect.top = 0;
	m_windowSizeRect.bottom = PER_DEFAULT_WINDOW_HEIGHT - PER_DEFAULT_WINDOW_BAR_HEIGHT;

	HDC hDC;
	hDC = GetDC(hWnd);

	// 더블 버퍼링 관련 설정
	m_memoryDC = CreateCompatibleDC(hDC);
	m_uiMemoryDC = CreateCompatibleDC(hDC);

	DeleteDC(hDC);
}

PERRenderer::~PERRenderer()
{
	PERLog::Logger().Info("렌더러 삭제");

	DeleteDC(m_memoryDC);
	DeleteDC(m_uiMemoryDC);
}

void PERRenderer::MatchWindowSize(HWND hWnd)
{
	GetClientRect(hWnd, &m_windowSizeRect);
}

void PERRenderer::SetCameraPosition(PERVec2 pos)
{
	m_cameraPosition = pos;
}

void PERRenderer::SetCameraSightWidth(double width)
{
	m_cameraSightWidth = width;
}


void PERRenderer::ResetMemoryDC(HWND hWnd)
{
	HDC hDC;
	hDC = GetDC(hWnd);

	// 메모리 dc 설정
	m_newBitmap = CreateCompatibleBitmap(hDC, m_windowSizeRect.right, m_windowSizeRect.bottom);
	m_oldBitmap = (HBITMAP)SelectObject(m_memoryDC, m_newBitmap);

	// 전체 흰색 초기화
	Rectangle(m_memoryDC, -1, -1, m_windowSizeRect.right + 1, m_windowSizeRect.bottom + 1);

	// 오브젝트 삭제
	DeleteObject(hDC);
}

void PERRenderer::ResetUIMemoryDC(HWND hWnd)
{
	HDC hDC;
	hDC = GetDC(hWnd);

	// 메모리 dc 설정
	m_uiNewBitmap = CreateCompatibleBitmap(hDC, m_windowSizeRect.right, m_windowSizeRect.bottom);
	m_uiOldBitmap = (HBITMAP)SelectObject(m_uiMemoryDC, m_uiNewBitmap);

	HBRUSH newBrush, oldBrush;
	newBrush = CreateSolidBrush(PER_TRANSPARENT_COLOR);
	oldBrush = (HBRUSH)SelectObject(m_uiMemoryDC, newBrush);

	// 전체 흰색 초기화
	Rectangle(m_uiMemoryDC, -1, -1, m_windowSizeRect.right + 1, m_windowSizeRect.bottom + 1);

	SelectObject(m_uiMemoryDC, oldBrush);
	DeleteObject(newBrush);
	DeleteObject(oldBrush);

	// 오브젝트 삭제
	DeleteObject(hDC);
}

void PERRenderer::FillHDCWithMemoryDCs(HWND hWnd)
{
	HDC hDC;
	hDC = GetDC(hWnd);

	TransparentBlt(m_memoryDC, 0, 0, m_windowSizeRect.right, m_windowSizeRect.bottom, m_uiMemoryDC,
		0, 0, m_windowSizeRect.right, m_windowSizeRect.bottom, PER_TRANSPARENT_COLOR);
	BitBlt(hDC, 0, 0, m_windowSizeRect.right, m_windowSizeRect.bottom, m_memoryDC, 0, 0, SRCCOPY);

	DeleteObject(hDC);
	SelectObject(m_memoryDC, m_oldBitmap);
	SelectObject(m_uiMemoryDC, m_uiOldBitmap);

	DeleteObject(m_newBitmap);
	DeleteObject(m_uiNewBitmap);
	DeleteObject(m_oldBitmap);
	DeleteObject(m_uiOldBitmap);

}

void PERRenderer::RenderShapeInWorldCoordinate(PERShapeType type, PERVec3 pos, PERVec3 size, PERColor color, 
	bool border, int borderWidth, PERColor borderColor)
{
	// 위치 좌표 변환, 크기 화면에 맞추어 변경
	PERVec2 posInVec2 = ConvertWorldCoordinateOpenGLToWindows(PERVec2(pos.x, pos.y));
	PERVec2 sizeInVec2 = MatchSizeWithWorldCoordinate(PERVec2(size.x, size.y));

	RenderShape(type, posInVec2, sizeInVec2, color, border, borderWidth, borderColor, m_memoryDC);
}

void PERRenderer::RenderFontInWorldCoordinate(const wchar_t* text, int textSize, double size, PERVec2 pos, PERColor color)
{
	pos = ConvertWorldCoordinateOpenGLToWindows(pos);
	size = MatchSizeWithWorldCoordinate(size);

	RenderFont(text, textSize, size, pos, color, m_memoryDC);
}

void PERRenderer::RenderShapeInScreenCoordinate(PERShapeType type, PERVec2 pos, PERVec2 size, PERColor color, 
	bool border, int borderWidth, PERColor borderColor)
{
	pos = ConvertScreenCoordinateOpenGLToWindows(pos);
	size = MatchSizeWithScreenCoordinate(size);

	RenderShape(type, pos, size, color, border, borderWidth, borderColor, m_uiMemoryDC);
}

void PERRenderer::RenderFontInScreenCoordinate(const wchar_t* text, int textSize, double size, PERVec2 pos, PERColor color)
{
	pos = ConvertScreenCoordinateOpenGLToWindows(pos);
	size = MatchSizeWithScreenCoordinate(size);

	RenderFont(text, textSize, size, pos, color, m_uiMemoryDC);
}

void PERRenderer::RenderShape(PERShapeType type, PERVec2 pos, PERVec2 size, PERColor color,
	bool border, int borderWidth, PERColor borderColor, HDC memDC)
{
	HBRUSH newBrush, oldBrush;
	newBrush = CreateSolidBrush(RGB(color.r, color.g, color.b));
	oldBrush = (HBRUSH)SelectObject(memDC, newBrush);

	HPEN newPen, oldPen;
	if (border) newPen = CreatePen(PS_SOLID, borderWidth, RGB(borderColor.r, borderColor.g, borderColor.b));
	else newPen = CreatePen(PS_SOLID, 0, RGB(color.r, color.g, color.b));
	oldPen = (HPEN)SelectObject(memDC, newPen);

	switch (type) {
	case PERShapeType::ELLIPSE:
		RenderEllipse(pos, size, memDC);
		break;
	case PERShapeType::RECTANGLE:
		RenderRectangle(pos, size, memDC);
		break;
	case PERShapeType::ROUND_RECTANGLE:
		RenderRoundRectangle(pos, size, 20, 20, memDC);
		break;
	case PERShapeType::TRIANGLE:
		RenderTriangle(pos, size, memDC);
		break;

	// 왼쪽 위 앵커 용
	case PERShapeType::ELLIPSE_WITH_LEFT_TOP_ANCHOR:
		RenderEllipseWithLeftTopAnchor(pos, size, memDC);
		break;
	case PERShapeType::RECTANGLE_WITH_LEFT_TOP_ANCHOR:
		RenderRectangleWithLeftTopAnchor(pos, size, memDC);
		break;
	case PERShapeType::ROUND_RECTANGLE_WITH_LEFT_TOP_ANCHOR:
		RenderRoundRectangleWithLeftTopAnchor(pos, size, 20, 20, memDC);
		break;
	case PERShapeType::TRIANGLE_WITH_LEFT_TOP_ANCHOR:
		RenderTriangleWithLeftTopAnchor(pos, size, memDC);
		break;
	}

	SelectObject(memDC, oldBrush);
	DeleteObject(newBrush);
	DeleteObject(oldBrush);

	SelectObject(memDC, oldPen);
	DeleteObject(newPen);
	DeleteObject(oldPen);
}

void PERRenderer::RenderFont(const wchar_t* text, int textSize, double size, PERVec2 pos, PERColor color, HDC memDC)
{
	HPEN newPen, oldPen;
	HFONT newFont, oldFont;

	newPen = (HPEN)GetStockObject(NULL_PEN);  // 투명선
	oldPen = (HPEN)SelectObject(m_memoryDC, newPen);
	newFont = CreateFont((int)size, 0, 0, 0,
		FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"맑은 고딕");
	SetTextColor(memDC, RGB(color.r, color.g, color.b));
	SetBkMode(memDC, TRANSPARENT);
	oldFont = (HFONT)SelectObject(memDC, newFont);

	TextOut(memDC, (int)pos.x, (int)pos.y, text, textSize);

	SelectObject(memDC, oldFont);
	DeleteObject(newPen);
	DeleteObject(oldFont);
	SelectObject(memDC, oldPen);
	DeleteObject(newFont);
	DeleteObject(oldPen);
}

void PERRenderer::RenderEllipse(PERVec2 pos, PERVec2 size, HDC memDC)
{
	Ellipse(memDC, (int)(pos.x - size.x / 2), (int)(pos.y - size.y / 2), (int)(pos.x + size.x / 2), (int)(pos.y + size.y / 2));
}

void PERRenderer::RenderRectangle(PERVec2 pos, PERVec2 size, HDC memDC)
{
	Rectangle(memDC, (int)(pos.x - size.x / 2), (int)(pos.y - size.y / 2), (int)(pos.x + size.x / 2), (int)(pos.y + size.y / 2));
}

void PERRenderer::RenderRoundRectangle(PERVec2 pos, PERVec2 size, int widthAngle, int heightAngle, HDC memDC)
{
	RoundRect(memDC, (int)(pos.x - size.x / 2), (int)(pos.y - size.y / 2),
		(int)(pos.x + size.x / 2), (int)(pos.y + size.y / 2), widthAngle, heightAngle);
}

void PERRenderer::RenderTriangle(PERVec2 pos, PERVec2 size, HDC memDC)
{
	POINT vertices[] = {
		{ (long)pos.x,				  (long)(pos.y - size.y / 2) },
		{ (long)(pos.x - size.x / 2), (long)(pos.y + size.y / 2) },
		{ (long)(pos.x + size.x / 2), (long)(pos.y + size.y / 2) }
	};
	Polygon(memDC, vertices, sizeof(vertices) / sizeof(vertices[0]));
}

void PERRenderer::RenderEllipseWithLeftTopAnchor(PERVec2 pos, PERVec2 size, HDC memDC)
{
	Ellipse(memDC, (int)(pos.x), (int)(pos.y), (int)(pos.x + size.x), (int)(pos.y + size.y));
}

void PERRenderer::RenderRectangleWithLeftTopAnchor(PERVec2 pos, PERVec2 size, HDC memDC)
{
	Rectangle(memDC, (int)(pos.x), (int)(pos.y), (int)(pos.x + size.x), (int)(pos.y + size.y));
}

void PERRenderer::RenderRoundRectangleWithLeftTopAnchor(PERVec2 pos, PERVec2 size, int widthAngle, int heightAngle, HDC memDC)
{
	RoundRect(memDC, (int)(pos.x), (int)(pos.y),
		(int)(pos.x + size.x), (int)(pos.y + size.y), widthAngle, heightAngle);
}

void PERRenderer::RenderTriangleWithLeftTopAnchor(PERVec2 pos, PERVec2 size, HDC memDC)
{
	POINT vertices[] = {
		{ (long)(pos.x + size.x / 2),	(long)(pos.y - size.y) },
		{ (long)(pos.x),				(long)(pos.y) },
		{ (long)(pos.x + size.x),		(long)(pos.y) }
	};
	Polygon(memDC, vertices, sizeof(vertices) / sizeof(vertices[0]));
}

PERVec2 PERRenderer::ConvertWorldCoordinateOpenGLToWindows(PERVec2 pos)
{
	pos.y *= -1;

	double cameraSightHeight = m_cameraSightWidth * (double)m_windowSizeRect.bottom / (double)m_windowSizeRect.right;

	double widthSizePerMeter  = (double)m_windowSizeRect.right / m_cameraSightWidth;
	double heightSizePerMeter = (double)m_windowSizeRect.bottom / cameraSightHeight;

	pos.x -= m_cameraPosition.x;
	pos.x += m_cameraSightWidth / 2.0;
	pos.x *= widthSizePerMeter;
	pos.y += m_cameraPosition.y;
	pos.y += cameraSightHeight / 2.0;
	pos.y *= heightSizePerMeter;

	return pos;
}

PERVec2 PERRenderer::MatchSizeWithWorldCoordinate(PERVec2 size)
{
	double cameraSightHeight = m_cameraSightWidth * (double)m_windowSizeRect.bottom / (double)m_windowSizeRect.right;

	double widthSizePerMeter  = (double)m_windowSizeRect.right / m_cameraSightWidth;
	double heightSizePerMeter = (double)m_windowSizeRect.bottom / cameraSightHeight;

	size.x *= widthSizePerMeter;
	size.y *= heightSizePerMeter;

	return size;
}

double PERRenderer::MatchSizeWithWorldCoordinate(double size)
{
	double cameraSightHeight = m_cameraSightWidth * (double)m_windowSizeRect.bottom / (double)m_windowSizeRect.right;

	double width  = (double)m_windowSizeRect.right / m_cameraSightWidth;
	double height = (double)m_windowSizeRect.bottom / cameraSightHeight;

	size *= (width + height) / 2.0;

	return size;
}

PERVec2 PERRenderer::ConvertScreenCoordinateOpenGLToWindows(PERVec2 pos)
{
	pos.y *= -1;

	pos.x += 1.0;
	pos.x *= (double)m_windowSizeRect.right / 2.0;
	pos.y += 1.0;
	pos.y *= (double)m_windowSizeRect.bottom / 2.0;

	return pos;
}

PERVec2 PERRenderer::MatchSizeWithScreenCoordinate(PERVec2 pos)
{
	pos.x *= (double)m_windowSizeRect.right / 2.0;
	pos.y *= (double)m_windowSizeRect.bottom / 2.0;

	return pos;
}

double PERRenderer::MatchSizeWithScreenCoordinate(double size)
{
	size *= (double)(m_windowSizeRect.bottom / 4.0 + m_windowSizeRect.right / 4.0);

	return size;
}
