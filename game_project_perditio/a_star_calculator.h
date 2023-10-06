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
	// 도착까지 남은 거리 계산
	int CalulateDistanceFromDest(int ax, int ay);
	void ChangeParentsToPaths();

	static const int c_MAX_CELL = 100;
	const double c_CELL_DISTANCE = 0.25;

	// 해당 좌표까지 이동하는 비용
	int m_costs[3][3] = {
		{14,	10,		14},
		{10,	0,		10},
		{14,	10,		10}
	};

	// 해당 좌표를 이미 방문했는 지 여부
	bool m_alreadyVisited[c_MAX_CELL][c_MAX_CELL];
	// 출발지부터 해당 좌표를 거처 도착까지의 추정 거리
	int m_distanceWithCell[c_MAX_CELL][c_MAX_CELL];

	// 우선순위 큐
	std::priority_queue<CellData*, std::vector<CellData*>, CellDataCmp> m_priorityQueue;

	// 출발 지점 좌표
	int m_startXIndexed, m_startYIndexed;
	// 도착 지점 좌표
	int m_destXIndexed, m_destYIndexed;
	
	// 도착지부터 시작해 처음 지점을 찾기 위한 부모 저장
	Cell* m_parents[c_MAX_CELL][c_MAX_CELL];
	// 최종 경로
	Cell m_paths[c_MAX_CELL];
	int m_numPath = 0;
};