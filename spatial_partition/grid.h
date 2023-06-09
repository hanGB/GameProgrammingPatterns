#pragma once
#include "unit.h"

class Grid {
public:
	Grid();

	void Add(Unit* unit);
	void Move(Unit* unit, double x, double y);
	void HandleMelee();

	void ShowEachCellCount();

	void RenderGrid(HDC& memDC, CoordinateData& cd);

	static const int c_NUM_CELLS = 10;
	static const int c_MIDDLE_CELL_INDEX = 5;
	static const int c_CELL_SIZE = 20;
	static const int c_ATTACK_DISTANCE_POWER_2 = 64;

private:
	void HandleCell(Unit* unit);
	void HandleAttack(Unit* unit, Unit* other);
	
	void FindCellWithPosition(double posX, double posY, int* cellX, int* cellY);
	double DistancePower2(Unit* unit, Unit* other);

	Unit* m_cells[c_NUM_CELLS][c_NUM_CELLS];
};