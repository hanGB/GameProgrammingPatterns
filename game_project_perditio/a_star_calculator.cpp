#include "stdafx.h"
#include "a_star_calculator.h"
#include "black_board.h"

AStarCalculator::AStarCalculator()
{
	// �湮 ���� �ʱ�ȭ
	memset(m_alreadyVisited, 0, c_MAX_CELL * c_MAX_CELL * sizeof(bool));
	// �ش� ���� ������ �Ÿ� �ʱ�ȭ
	for (int x = 0; x < c_MAX_CELL; ++x) {
		for (int y = 0; y < c_MAX_CELL; ++y) {
			m_distanceWithCell[x][y] = INT_MAX;
		}
	}
}

AStarCalculator::~AStarCalculator()
{
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

void AStarCalculator::SetStartAndDestination(PERVec3 start, PERVec3 dest)
{
	// �� ��ǥ�� �ε���ȭ
	m_startXIndexed = (int)(start.x / c_CELL_DISTANCE) + (c_MAX_CELL / 2);
	m_startYIndexed = (int)(start.y / c_CELL_DISTANCE) + (c_MAX_CELL / 2);
	m_destXIndexed = (int)(dest.x / c_CELL_DISTANCE) + (c_MAX_CELL / 2);
	m_destYIndexed = (int)(dest.y / c_CELL_DISTANCE) + (c_MAX_CELL / 2);

	// ���� �Ÿ� ����
	m_distanceWithCell[m_startXIndexed][m_startYIndexed] = CalulateDistanceFromDest(m_startXIndexed, m_startYIndexed);

	// ����� ť���� ������
	if (m_cellDataQueue.empty()) m_cellDataQueue.push(new CellData());
	CellData* cData = m_cellDataQueue.front();
	m_cellDataQueue.pop();

	cData->x = m_startXIndexed; cData->y = m_startYIndexed;
	cData->f = m_distanceWithCell[m_startXIndexed][m_startYIndexed];
	cData->g = 0;

	m_priorityQueue.push(cData);

	// ����� ť���� ������
	if (m_cellQueue.empty()) m_cellQueue.push(new Cell());
	m_parents[m_startXIndexed][m_startYIndexed] = m_cellQueue.front();
	m_parents[m_startXIndexed][m_startYIndexed]->SetData(m_startXIndexed, m_startYIndexed);
	m_cellQueue.pop();

	PERLog::Logger().InfoWithFormat("������ - %d, %d", m_startXIndexed, m_startYIndexed);
	PERLog::Logger().InfoWithFormat("������ - %d, %d", m_destXIndexed, m_destYIndexed);
}

void AStarCalculator::FindPath()
{
	while (m_priorityQueue.size() > 0) {
		CellData* data = m_priorityQueue.top();
		m_priorityQueue.pop();

		// �̹� �湮�� ��� ��ŵ
		if (m_alreadyVisited[data->x][data->y]) {
			m_cellDataQueue.push(data);
			continue;
		}
		
		//PERLog::Logger().Info("ó���Ǿ���?");

		// �湮 ó��
		m_alreadyVisited[data->x][data->y] = true;

		// ���������� ������ ����
		if (data->x == m_destXIndexed && data->y == m_destYIndexed) break;

		// �ֺ� �˻�
		for (int x = -1; x < 2; ++x) {
			for (int y = -1; y < 2; ++y) {

				//PERLog::Logger().Info("�ֺ� �˻� ��");

				// ���� ��ǥ
				int nextX = data->x + x;
				int nextY = data->y + y;

				// ������ �ش� �Ǵ� ��� ��ŵ
				// �� ũ�� ��
				if (nextX < 0 || nextX > c_MAX_CELL || nextY < 0 || nextY > c_MAX_CELL) continue;
				// �� �� ���� ��(���� �ƴ� ��)
				if (BlackBoard::GetNavigationData().GetCellInfo(nextX, nextY) != NavigationCellType::GROUND) continue;
				// �̹� �湮�� ��
				if (m_alreadyVisited[nextX][nextY]) continue;

				// ���� �Ÿ�(���) ���
				int g = data->g + m_costs[x + 1][y + 1];
				int h = CalulateDistanceFromDest(nextX, nextY);

				// �̹� �� ���� ��찡 ������ ��ŵ
				if (m_distanceWithCell[nextX][nextY] < g + h) continue;

				//PERLog::Logger().InfoWithFormat("%d, %d ����", nextX, nextY);

				// ����
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

void AStarCalculator::ClearPaths()
{
	// �θ� ��� �ʱ�ȭ
	for (int x = 0; x < c_MAX_CELL; ++x) {
		for (int y = 0; y < c_MAX_CELL; ++y) {
			if (!m_parents[x][y]) continue;
			m_cellQueue.push(m_parents[x][y]);
			m_parents[x][y] = nullptr;
		}
	}

	// �湮 ���� �ʱ�ȭ
	memset(m_alreadyVisited, 0, c_MAX_CELL * c_MAX_CELL * sizeof(bool));
	// �ش� ���� ������ �Ÿ� �ʱ�ȭ
	for (int x = 0; x < c_MAX_CELL; ++x) {
		for (int y = 0; y < c_MAX_CELL; ++y) {
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
		// �������� �̵��� �Ÿ�
		int str = (distX - distY);
		// �밢�� �̵��� �Ÿ� * ���
		int dia = 14 * (distX - str);
		// ���� �̵� ���
		str *= 10;

		cost = dia + str;
	}
	else if (distX < distY) {
		// �������� �̵��� �Ÿ�
		int str = (distY - distX);
		// �밢�� �̵��� �Ÿ� * ���
		int dia = 14 * (distY - str);
		// ���� �̵� ���
		str *= 10;

		cost = dia + str;
	}
	else 
	{
		// �밢�����θ� �̵�
		cost = 14 * distX;
	}

	return cost;
}

void AStarCalculator::ChangeParentsToPaths()
{
	int x = m_destXIndexed, y = m_destYIndexed;

	// ���������� �� ������ �θ� ���� ��������� ����
	m_numPath = 0;
	while (m_parents[x][y]->x != x || m_parents[x][y]->y != y) {
		m_paths[m_numPath++] = Cell(x, y);
		Cell* cell = m_parents[x][y];
		x = cell->x;
		y = cell->y;
	}
	m_paths[m_numPath++] = Cell(x, y);

	// ��������� ���ĵǵ��� ������
	for (int i = 0; i < m_numPath / 2; ++i) {
		Cell temp = m_paths[m_numPath -1 -i];
		m_paths[m_numPath -1 - i] = m_paths[i];
		m_paths[i] = temp;
	}
}
