#pragma once

class PERObject;

enum class NavigationCellType {
	NOTHING,
	GROUND,
	WALL
};

class NavigationData {
public:
	NavigationData();
	~NavigationData();

	void InitCells();
	void SetCells(std::vector<PERObject*>& objects, int numObject);
	// �׽�Ʈ�� �ؽ�Ʈ ���Ϸ� ���
	void TextOutData();

	NavigationCellType GetCellInfo(int x, int y) const;

private:
	static const int c_MAX_CELL = 100;
	const double c_CELL_DISTANCE = 0.25;

	NavigationCellType m_cells[c_MAX_CELL][c_MAX_CELL];
};