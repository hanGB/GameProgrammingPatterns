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
	NavigationCellType m_cells[PER_MAX_CELL][PER_MAX_CELL];
};