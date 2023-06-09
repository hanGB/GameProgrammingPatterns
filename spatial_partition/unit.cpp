#include "stdafx.h"
#include "unit.h"
#include "grid.h"

Unit::Unit(Grid* grid, double x, double y)
	: m_grid(grid), m_x(x), m_y(y), m_prev(nullptr), m_next(nullptr)
{
	m_grid->Add(this);
}

void Unit::MoveTo(double x, double y)
{
	m_grid->Move(this, x, y);
}

void Unit::MoveBy(double x, double y)
{
	m_grid->Move(this, m_x + x, m_y + y);
}

void Unit::Render(HDC& memDC, CoordinateData& cd)
{
	HBRUSH newBrush, oldBrush;
	int color = 255 - m_attack_index * c_DARKNESS_VALUE;
	if (color < 0) color = 0;
	newBrush = CreateSolidBrush(RGB(color, color, color));
	oldBrush = (HBRUSH)SelectObject(memDC, newBrush);

	double winX = m_x, winY = m_y;
	cd.ConvertCoordinateOpenGLToWindows(&winX, &winY);

	Rectangle(memDC,
		static_cast<int>((winX - c_UNIT_SIZE / 2.0) * PIXEL_PER_METER),
		static_cast<int>((winY - c_UNIT_SIZE / 2.0) * PIXEL_PER_METER),
		static_cast<int>((winX + c_UNIT_SIZE / 2.0) * PIXEL_PER_METER),
		static_cast<int>((winY + c_UNIT_SIZE / 2.0) * PIXEL_PER_METER)
	);

	(HPEN)SelectObject(memDC, oldBrush);
	DeleteObject(newBrush);

	m_attack_index = 0;
}
