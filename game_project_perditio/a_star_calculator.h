#pragma once
#include <queue>

namespace AStar {
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
}

class AStarCalculator {
public:
	AStarCalculator();
	~AStarCalculator();

	bool FindPath(PERVec3 start, PERVec3 dest, PERVec3* paths, int* numPath);

private: 
	// 패스 계산
	void SetStartAndDestination(PERVec3 start, PERVec3 dest);
	bool CalculateParents();
	void ChangeParentsToPaths(PERVec3* paths, int* numPath);
	void Clear();

	// 도착까지 남은 거리 계산
	int CalulateDistanceFromDest(int ax, int ay);

	// 해당 좌표까지 이동하는 비용
	int m_costs[3][3] = {
		{14,	10,		14},
		{10,	0,		10},
		{14,	10,		14}
	};

	// 해당 좌표를 이미 방문했는 지 여부
	bool m_alreadyVisited[PER_MAX_CELL][PER_MAX_CELL];
	// 출발지부터 해당 좌표를 거처 도착까지의 추정 거리
	int m_distanceWithCell[PER_MAX_CELL][PER_MAX_CELL];

	// 우선순위 큐
	std::priority_queue<AStar::CellData*, std::vector<AStar::CellData*>, AStar::CellDataCmp> m_priorityQueue;

	// 출발 지점 좌표
	int m_startXIndexed, m_startYIndexed;
	// 도착 지점 좌표
	int m_destXIndexed, m_destYIndexed;
	
	// 도착지부터 시작해 처음 지점을 찾기 위한 부모 저장
	AStar::Cell* m_parents[PER_MAX_CELL][PER_MAX_CELL];

	// 생성, 삭제를 줄이기 위한 저장소
	std::queue<AStar::Cell*> m_cellQueue;
	std::queue<AStar::CellData*> m_cellDataQueue;
};