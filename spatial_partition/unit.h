#pragma once

class Unit {
	friend class Grid;

public:
	Unit(Grid* grid, double x, double y);
	
	// �ش� ��ǥ�� �̵�
	void MoveTo(double x, double y);
	// �ش� ����ŭ �̵�
	void MoveBy(double x, double y);
	void Render(HDC& memDC, CoordinateData& cd);

private:
	const int c_UNIT_SIZE = 2;
	const int c_DARKNESS_VALUE = 20;

	double m_x, m_y;
	int m_attack_index = 0;
	Grid* m_grid;

	Unit* m_prev;
	Unit* m_next;
};