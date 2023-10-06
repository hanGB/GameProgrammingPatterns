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

	int x;
	int y;
};

class AStarCalculator {
public:
	AStarCalculator();
	~AStarCalculator();

	void SetStartAndDestination(PERVec3 start, PERVec3 dest);
	void FindPath();

	Cell* GetPaths();
	int GetNumPath() const;

private: 
	// �������� ���� �Ÿ� ���
	int CalulateDistanceFromDest(int ax, int ay);
	void ChangeParentsToPaths();

	static const int c_MAX_CELL = 100;
	const double c_CELL_DISTANCE = 0.25;

	// �ش� ��ǥ���� �̵��ϴ� ���
	int m_costs[3][3] = {
		{14,	10,		14},
		{10,	0,		10},
		{14,	10,		10}
	};

	// �ش� ��ǥ�� �̹� �湮�ߴ� �� ����
	bool m_alreadyVisited[c_MAX_CELL][c_MAX_CELL];
	// ��������� �ش� ��ǥ�� ��ó ���������� ���� �Ÿ�
	int m_distanceWithCell[c_MAX_CELL][c_MAX_CELL];

	// �켱���� ť
	std::priority_queue<CellData*, std::vector<CellData*>, CellDataCmp> m_priorityQueue;

	// ��� ���� ��ǥ
	int m_startXIndexed, m_startYIndexed;
	// ���� ���� ��ǥ
	int m_destXIndexed, m_destYIndexed;
	
	// ���������� ������ ó�� ������ ã�� ���� �θ� ����
	Cell* m_parents[c_MAX_CELL][c_MAX_CELL];
	// ���� ���
	Cell m_paths[c_MAX_CELL];
	int m_numPath = 0;
};