#include "stdafx.h"
#include "a_star_calculator.h"
#include "black_board.h"

AStarCalculator::AStarCalculator()
{
	Clear();
}

AStarCalculator::~AStarCalculator()
{
	Clear();

	while (!m_cellQueue.empty()) {
		Cell* cell = m_cellQueue.front();
		m_cellQueue.pop();
		delete cell;
	}
	while (!m_cellDataQueue.empty()) {
		CellData* cellData = m_cellDataQueue.front();
		m_cellDataQueue.pop();
		delete cellData;
	}
}

bool AStarCalculator::FindPath(PERVec3 start, PERVec3 dest, PERVec3* paths, int* numPath)
{
	// 시작 및 도작점 설정
	SetStartAndDestination(start, dest);
	// 경로 도출을 위한 부모 목록 계산(경로 검색에 실패했는 지 또한 반환)
	bool result = CalculateParents();
	// 부모 목록을 실제 경로로 변경
	if (result) ChangeParentsToPaths(paths, numPath);
	// 초기화
	Clear();

	return result;
}

void AStarCalculator::SetStartAndDestination(PERVec3 start, PERVec3 dest)
{
	// 각 좌표를 인덱스화
	m_startXIndexed = (int)(start.x / PER_CELL_DISTANCE) + (PER_MAX_CELL / 2);
	m_startYIndexed = (int)(start.y / PER_CELL_DISTANCE) + (PER_MAX_CELL / 2);
	m_destXIndexed = (int)(dest.x / PER_CELL_DISTANCE) + (PER_MAX_CELL / 2);
	m_destYIndexed = (int)(dest.y / PER_CELL_DISTANCE) + (PER_MAX_CELL / 2);

	// 추정 거리 저장
	m_distanceWithCell[m_startXIndexed][m_startYIndexed] = CalulateDistanceFromDest(m_startXIndexed, m_startYIndexed);

	// 저장된 큐에서 꺼내옴
	if (m_cellDataQueue.empty()) m_cellDataQueue.push(new CellData());
	CellData* cData = m_cellDataQueue.front();
	m_cellDataQueue.pop();

	cData->x = m_startXIndexed; cData->y = m_startYIndexed;
	cData->f = m_distanceWithCell[m_startXIndexed][m_startYIndexed];
	cData->g = 0;

	m_priorityQueue.push(cData);

	// 저장된 큐에서 꺼내옴
	if (m_cellQueue.empty()) m_cellQueue.push(new Cell());
	m_parents[m_startXIndexed][m_startYIndexed] = m_cellQueue.front();
	m_parents[m_startXIndexed][m_startYIndexed]->SetData(m_startXIndexed, m_startYIndexed);
	m_cellQueue.pop();

	//PERLog::Logger().InfoWithFormat("시작점 - %d, %d", m_startXIndexed, m_startYIndexed);
	//PERLog::Logger().InfoWithFormat("도착점 - %d, %d", m_destXIndexed, m_destYIndexed);
}

bool AStarCalculator::CalculateParents()
{
	while (m_priorityQueue.size() > 0) {
		CellData* data = m_priorityQueue.top();
		m_priorityQueue.pop();

		// 이미 방문한 경우 스킵
		if (m_alreadyVisited[data->x][data->y]) {
			m_cellDataQueue.push(data);
			continue;
		}
		// 방문 처리
		m_alreadyVisited[data->x][data->y] = true;

		// 도착했으니 성공 리턴
		if (data->x == m_destXIndexed && data->y == m_destYIndexed) return true; 

		// 주변 검색
		for (int x = -1; x < 2; ++x) {
			for (int y = -1; y < 2; ++y) {

				//PERLog::Logger().Info("주변 검색 중");

				// 다음 좌표
				int nextX = data->x + x;
				int nextY = data->y + y;

				// 다음에 해당 되는 경우 스킵
				// 맵 크기 밖
				if (nextX < 0 || nextX >= PER_MAX_CELL || nextY < 0 || nextY >= PER_MAX_CELL) continue;
				// 갈 수 없는 곳(땅이 아닌 곳)
				if (BlackBoard::GetNavigationData().GetCellInfo(nextX, nextY) != NavigationCellType::GROUND) continue;
				// 이미 방문한 셀
				if (m_alreadyVisited[nextX][nextY]) continue;
				// 대각선으로 갈 수 없는 경우(이동하면서 지나가는 지점이 땅이 아님)
				if (m_costs[x + 1][y + 1] == 14)
				{
					// x, y 중 둘 중 하나가 0일 경우의 칸이 땅이 아닌 경우 스킵
					if (BlackBoard::GetNavigationData().GetCellInfo(nextX, data->y) != NavigationCellType::GROUND) continue;
					if (BlackBoard::GetNavigationData().GetCellInfo(data->x, nextY) != NavigationCellType::GROUND) continue;
				}

				// 가는 거리(비용) 계산
				int g = data->g + m_costs[x + 1][y + 1];
				int h = CalulateDistanceFromDest(nextX, nextY);

				// 이미 더 좋은 경우가 있으면 스킵
				if (m_distanceWithCell[nextX][nextY] < g + h) continue;

				//PERLog::Logger().InfoWithFormat("%d, %d 저장", nextX, nextY);

				// 저장
				m_distanceWithCell[nextX][nextY] = g + h;

				if (m_cellDataQueue.empty()) m_cellDataQueue.push(new CellData());
				CellData* nextCell = m_cellDataQueue.front();
				m_cellDataQueue.pop();
				nextCell->x = nextX;
				nextCell->y = nextY;
				nextCell->g = g;
				nextCell->f = g + h;

				m_priorityQueue.push(nextCell);

				if (m_cellQueue.empty()) m_cellQueue.push(new Cell());
				m_parents[nextX][nextY] = m_cellQueue.front();
				m_parents[nextX][nextY]->SetData(data->x, data->y);
				m_cellQueue.pop();
			}
		}
	}

	// 도착을 못 했으니 실패 리턴
	return false;
}

void AStarCalculator::ChangeParentsToPaths(PERVec3* paths, int* numPath)
{
	int x = m_destXIndexed, y = m_destYIndexed;

	// 도착지부터 그 지점의 부모를 따라 출발지까지 저장
	*numPath = 0;
	while (m_parents[x][y]->x != x || m_parents[x][y]->y != y) {
		paths[(*numPath)++] = BlackBoard::GetNavigationData().ChangeCellToWorldPosition(x, y);
		// 현재 지점의 부모(그 지점으로 가기 전 지점)로 이동
		Cell* parent = m_parents[x][y];
		x = parent->x;
		y = parent->y;
	}
	paths[(*numPath)++] = BlackBoard::GetNavigationData().ChangeCellToWorldPosition(x, y);

	// 출발지부터 정렬되도록 역정렬
	for (int i = 0; i < *numPath / 2; ++i) {
		PERVec3 temp = paths[*numPath - 1 - i];
		paths[*numPath - 1 - i] = paths[i];
		paths[i] = temp;
	}
}

void AStarCalculator::Clear()
{
	// 우선순위 큐에 남은 거 제거
	while (!m_priorityQueue.empty()) {
		m_cellDataQueue.push(m_priorityQueue.top());
		m_priorityQueue.pop();
	}

	// 부모 목록 초기화
	for (int x = 0; x < PER_MAX_CELL; ++x) {
		for (int y = 0; y < PER_MAX_CELL; ++y) {
			if (!m_parents[x][y]) continue;
			m_cellQueue.push(m_parents[x][y]);
			m_parents[x][y] = nullptr;
		}
	}

	// 방문 여부 초기화
	memset(m_alreadyVisited, 0, PER_MAX_CELL * PER_MAX_CELL * sizeof(bool));
	// 해당 셀을 포함한 거리 초기화
	for (int x = 0; x < PER_MAX_CELL; ++x) {
		for (int y = 0; y < PER_MAX_CELL; ++y) {
			m_distanceWithCell[x][y] = INT_MAX;
		}
	}
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
