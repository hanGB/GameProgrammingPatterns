#pragma once
#include <queue>

struct CellData {
	int x;
	int y;
	int f;
	int g;

};

struct CellDataCmp {
	bool operator()(CellData* a, CellData* b) {
		return a->f > b->f;
	}
};

struct Cell {
	Cell() {}
	Cell(int x, int y) : x(x), y(y) {}

	void SetData(int x, int y) {
		this->x = x;
		this->y = y;
	}

	int x;
	int y;
};

class AStarCalculator {
public:
	AStarCalculator();
	~AStarCalculator();

	void FindPath(PERVec3 start, PERVec3 dest, Cell* paths, int* numPath);

private: 
	// �н� ���
	void SetStartAndDestination(PERVec3 start, PERVec3 dest);
	void CalculateParents();
	void ChangeParentsToPaths(Cell* paths, int* numPath);
	void Clear();

	// �������� ���� �Ÿ� ���
	int CalulateDistanceFromDest(int ax, int ay);

	// �ش� ��ǥ���� �̵��ϴ� ���
	int m_costs[3][3] = {
		{14,	10,		14},
		{10,	0,		10},
		{14,	10,		10}
	};

	// �ش� ��ǥ�� �̹� �湮�ߴ� �� ����
	bool m_alreadyVisited[PER_MAX_CELL][PER_MAX_CELL];
	// ��������� �ش� ��ǥ�� ��ó ���������� ���� �Ÿ�
	int m_distanceWithCell[PER_MAX_CELL][PER_MAX_CELL];

	// �켱���� ť
	std::priority_queue<CellData*, std::vector<CellData*>, CellDataCmp> m_priorityQueue;

	// ��� ���� ��ǥ
	int m_startXIndexed, m_startYIndexed;
	// ���� ���� ��ǥ
	int m_destXIndexed, m_destYIndexed;
	
	// ���������� ������ ó�� ������ ã�� ���� �θ� ����
	Cell* m_parents[PER_MAX_CELL][PER_MAX_CELL];

	// ����, ������ ���̱� ���� �����
	std::queue<Cell*> m_cellQueue;
	std::queue<CellData*> m_cellDataQueue;
};