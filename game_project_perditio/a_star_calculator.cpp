#include "stdafx.h"
#include "a_star_calculator.h"
#include "black_board.h"

AStarCalculator::AStarCalculator()
{
	// 방문 여부 초기화
	memset(m_alreadyVisited, 0, c_MAX_CELL * c_MAX_CELL * sizeof(bool));
	// 해당 셀을 포함한 거리 초기화
	for (int x = 0; x < c_MAX_CELL; ++x) {
		for (int y = 0; y < c_MAX_CELL; ++y) {
			m_distanceWithCell[x][y] = INT_MAX;
		}
	}
}

AStarCalculator::~AStarCalculator()
{
}

void AStarCalculator::SetStartAndDestination(PERVec3 start, PERVec3 dest)
{
	// 각 좌표를 인덱스화
	m_startXIndexed = (int)(start.x / c_CELL_DISTANCE) + (c_MAX_CELL / 2);
	m_startYIndexed = (int)(start.y / c_CELL_DISTANCE) + (c_MAX_CELL / 2);
	m_destXIndexed = (int)(dest.x / c_CELL_DISTANCE) + (c_MAX_CELL / 2);
	m_destYIndexed = (int)(dest.y / c_CELL_DISTANCE) + (c_MAX_CELL / 2);

	// 추정 거리 저장
	m_distanceWithCell[m_startXIndexed][m_startYIndexed] = CalulateDistanceFromDest(m_startXIndexed, m_startYIndexed);

	CellData* cData = new CellData();
	cData->x = m_startXIndexed; cData->y = m_startYIndexed;
	cData->f = CalulateDistanceFromDest(m_startXIndexed, m_startYIndexed);
	cData->g = 0;

	m_priorityQueue.push(cData);

	m_parents[m_startXIndexed][m_startYIndexed] = new Cell(m_startXIndexed, m_startYIndexed);

	PERLog::Logger().InfoWithFormat("시작점 - %d, %d", m_startXIndexed, m_startYIndexed);
	PERLog::Logger().InfoWithFormat("도착점 - %d, %d", m_destXIndexed, m_destYIndexed);
}

void AStarCalculator::FindPath()
{
	while (m_priorityQueue.size() > 0) {
		CellData* data = m_priorityQueue.top();
		m_priorityQueue.pop();

		// 이미 방문한 경우 스킵
		if (m_alreadyVisited[data->x][data->y]) {
			delete data;
			continue;
		}
		
		//PERLog::Logger().Info("처리되었나?");

		// 방문 처리
		m_alreadyVisited[data->x][data->y] = true;

		// 도착했으니 루프를 나감
		if (data->x == m_destXIndexed && data->y == m_destYIndexed) break;

		// 주변 검색
		for (int x = -1; x < 2; ++x) {
			for (int y = -1; y < 2; ++y) {

				//PERLog::Logger().Info("주변 검색 중");

				// 다음 좌표
				int nextX = data->x + x;
				int nextY = data->y + y;

				// 다음에 해당 되는 경우 스킵
				// 맵 크기 밖
				if (nextX < 0 || nextX > c_MAX_CELL || nextY < 0 || nextY > c_MAX_CELL) continue;
				// 갈 수 없는 곳(땅이 아닌 곳)
				if (BlackBoard::GetNavigationData().GetCellInfo(nextX, nextY) != NavigationCellType::GROUND) continue;
				// 이미 방문한 셀
				if (m_alreadyVisited[nextX][nextY]) continue;

				// 가는 거리(비용) 계산
				int g = data->g + m_costs[x + 1][y + 1];
				int h = CalulateDistanceFromDest(nextX, nextY);

				// 이미 더 좋은 경우가 있으면 스킵
				if (m_distanceWithCell[nextX][nextY] < g + h) continue;

				//PERLog::Logger().InfoWithFormat("%d, %d 저장", nextX, nextY);

				// 저장
				m_distanceWithCell[nextX][nextY] = g + h;
				CellData* nextCell = new CellData();
				nextCell->x = nextX;
				nextCell->y = nextY;
				nextCell->g = g;
				nextCell->f = g + h;

				m_priorityQueue.push(nextCell);
				m_parents[nextX][nextY] = new Cell(data->x, data->y);
			}
		}
	}

	ChangeParentsToPaths();
}

Cell* AStarCalculator::GetPaths()
{
	return m_paths;
}

int AStarCalculator::GetNumPath() const
{
	return m_numPath;
}

int AStarCalculator::CalulateDistanceFromDest(int x, int y)
{
	int distX = std::abs(m_destXIndexed - x);
	int distY = std::abs(m_destYIndexed - y);

	int cost;
	if (distX > distY) {
		// 직선으로 이동할 거리
		int str = (distX - distY);
		// 대각선 이동할 거리 * 비용
		int dia = 14 * (distX - str);
		// 직선 이동 비용
		str *= 10;

		cost = dia + str;
	}
	else if (distX < distY) {
		// 직선으로 이동할 거리
		int str = (distY - distX);
		// 대각선 이동할 거리 * 비용
		int dia = 14 * (distY - str);
		// 직선 이동 비용
		str *= 10;

		cost = dia + str;
	}
	else 
	{
		// 대각선으로만 이동
		cost = 14 * distX;
	}

	return cost;
}

void AStarCalculator::ChangeParentsToPaths()
{
	int x = m_destXIndexed, y = m_destYIndexed;

	// 도착지부터 그 지점의 부모를 따라 출발지까지 저장
	m_numPath = 0;
	while (m_parents[x][y]->x != x || m_parents[x][y]->y != y) {
		m_paths[m_numPath++] = Cell(x, y);
		Cell* cell = m_parents[x][y];
		x = cell->x;
		y = cell->y;
	}
	m_paths[m_numPath++] = Cell(x, y);

	// 출발지부터 정렬되도록 역정렬
	for (int i = 0; i < m_numPath / 2; ++i) {
		Cell temp = m_paths[m_numPath -1 -i];
		m_paths[m_numPath -1 - i] = m_paths[i];
		m_paths[i] = temp;
	}
}
