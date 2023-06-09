#include "stdafx.h"
#include "grid.h"

Grid::Grid()
{
	// 격자 초기화
	for (int x = 0; x < c_NUM_CELLS; ++x) {
		for (int y = 0; y < c_NUM_CELLS; ++y) {
			m_cells[x][y] = nullptr;
		}
	}
}

void Grid::Add(Unit* unit)
{
	// 어느 칸에 들어갈지 결정
	int cellX, cellY;
	FindCellWithPosition(unit->m_x, unit->m_y, &cellX, &cellY);

	// 칸에 들어 있는 리스트의 맨 앞에 추가
	unit->m_prev = nullptr;
	unit->m_next = m_cells[cellX][cellY];
	m_cells[cellX][cellY] = unit;

	if (unit->m_next != nullptr) {
		unit->m_next->m_prev = unit;
	}
}

void Grid::Move(Unit* unit, double x, double y)
{
	// 유닛이 어느 칸에 있었는지 확인
	int oldCellX, oldCellY; 
	FindCellWithPosition(unit->m_x, unit->m_y, &oldCellX, &oldCellY);

	// 유닛이 어느 칸으로 가야 하는지 확인
	int cellX, cellY;
	FindCellWithPosition(x, y, &cellX, &cellY);

	unit->m_x = x;
	unit->m_y = y;

	// 칸이 바뀌지 않았다면 종료
	if (oldCellX == cellX && oldCellY == cellY) {
		return;
	}

	// 이전 칸에 들어 있는 리스트에서 유닛 제거
	if (unit->m_prev != nullptr) {
		unit->m_prev->m_next = unit->m_next;
	}
	if (unit->m_next != nullptr) {
		unit->m_next->m_prev = unit->m_prev;
	}
	// 유닛이 칸에 들어 있는 리스트의 머리였다면 머리 변경
	if (m_cells[oldCellX][oldCellY] == unit) {
		m_cells[oldCellX][oldCellY] = unit->m_next;
	}

	// 새로 들어갈 칸에 추가
	Add(unit);
}

void Grid::HandleMelee()
{
	for (int x = 0; x < c_NUM_CELLS; ++x) {
		for (int y = 0; y < c_NUM_CELLS; ++y) {
			HandleCell(x, y);
		}
	}
}

void Grid::ShowEachCellCount()
{
	// 각 셀에 들어가 있는 유닛 수 확인
	for (int x = 0; x < c_NUM_CELLS; ++x) {
		for (int y = 0; y < c_NUM_CELLS; ++y) {
			Unit* cell = m_cells[x][y];
			int count = 0;
			while (cell != nullptr) {
				count++;
				cell = cell->m_next;
			}
			std::cout << count << " ";
		}
	}
	std::cout << '\n';
	std::cout << '\n';
	std::cout << '\n';
}

void Grid::HandleCell(int x, int y)
{
	Unit* unit = m_cells[x][y];

	while (unit != nullptr) {
		// 이 셀에 들어 있는 다른 유닛 처리
		HandleUnit(unit, unit->m_next);

		// 주변 칸에 들어 있는 유닛들도 확인
		if (x > 0) HandleUnit(unit, m_cells[x - 1][y]);
		if (y > 0) HandleUnit(unit, m_cells[x][y - 1]);
		if (x > 0 && y > 0) HandleUnit(unit, m_cells[x - 1][y - 1]);
		if (x > 0 && y < c_NUM_CELLS - 1) HandleUnit(unit, m_cells[x - 1][y + 1]);
		//if (x < c_NUM_CELLS - 1 && y > 0)  HandleUnit(unit, m_cells[x + 1][y - 1]);
		//if (x < c_NUM_CELLS - 1) HandleUnit(unit, m_cells[x + 1][y]);
		//if (y < c_NUM_CELLS - 1) HandleUnit(unit, m_cells[x][y + 1]);
		//if (x < c_NUM_CELLS - 1 && y < c_NUM_CELLS - 1)  HandleUnit(unit, m_cells[x + 1][y + 1]);

		unit = unit->m_next;
	}
}

void Grid::HandleUnit(Unit* unit, Unit* other)
{
	while (other != nullptr) {
		// 유닛 사이의 거리의 제곱과 공격 범위의 제곱을 비교
		if (DistancePower2(unit, other) < c_ATTACK_DISTANCE_POWER_2) {
			HandleAttack(unit, other);
		}
		other = other->m_next;
	}
}

void Grid::HandleAttack(Unit* unit, Unit* other)
{
	unit->m_attack_index++;
	other->m_attack_index++;
}

void Grid::FindCellWithPosition(double posX, double posY, int* cellX, int* cellY)
{
	if (posX >= 0.0) {
		*cellX = (int)(posX / c_CELL_SIZE) + c_MIDDLE_CELL_INDEX;
	}
	else {
		*cellX = (int)(-posX / c_CELL_SIZE);
	}
	if (posY >= 0.0) {
		*cellY = (int)(posY / c_CELL_SIZE) + c_MIDDLE_CELL_INDEX;
	}
	else {
		*cellY = (int)(-posY / c_CELL_SIZE);
	}
}

double Grid::DistancePower2(Unit* unit, Unit* other)
{
	double disX = unit->m_x - other->m_x;
	double disY = unit->m_y - other->m_y;

	return (disX * disX + disY * disY);
}

void Grid::RenderGrid(HDC& memDC, CoordinateData& cd)
{
	HPEN newPen, oldPen;
	newPen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
	oldPen = (HPEN)SelectObject(memDC, newPen);

	double sx, sy, ex, ey;
	for (int i = -c_MIDDLE_CELL_INDEX; i <= c_MIDDLE_CELL_INDEX; ++i) {
		sx = c_CELL_SIZE * i, sy = -c_CELL_SIZE * c_MIDDLE_CELL_INDEX * PIXEL_PER_METER;
		ex = c_CELL_SIZE * i, ey = c_CELL_SIZE * c_MIDDLE_CELL_INDEX * PIXEL_PER_METER;
		cd.ConvertCoordinateOpenGLToWindows(&sx, &sy);
		cd.ConvertCoordinateOpenGLToWindows(&ex, &ey);
		MoveToEx(memDC, (int)sx * PIXEL_PER_METER, (int)sy * PIXEL_PER_METER, NULL);
		LineTo(memDC, (int)sx * PIXEL_PER_METER, (int)ey * PIXEL_PER_METER);
	}
	for (int i = -c_MIDDLE_CELL_INDEX; i <= c_MIDDLE_CELL_INDEX; ++i) {
		sx = -c_CELL_SIZE * c_MIDDLE_CELL_INDEX * PIXEL_PER_METER, sy = c_CELL_SIZE * i;
		ex = c_CELL_SIZE * c_MIDDLE_CELL_INDEX * PIXEL_PER_METER, ey = c_CELL_SIZE * i;
		cd.ConvertCoordinateOpenGLToWindows(&sx, &sy);
		cd.ConvertCoordinateOpenGLToWindows(&ex, &ey);
		MoveToEx(memDC, (int)sx * PIXEL_PER_METER, (int)sy * PIXEL_PER_METER, NULL);
		LineTo(memDC, (int)ex * PIXEL_PER_METER, (int)ey * PIXEL_PER_METER);
	}

	(HPEN)SelectObject(memDC, oldPen);
	DeleteObject(newPen);
}

