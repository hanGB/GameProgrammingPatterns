#include "stdafx.h"
#include "navigation_data.h"
#include "per_object.h"
#include "per_renderer.h"

NavigationData::NavigationData()
{
	InitCells();
}

NavigationData::~NavigationData()
{
}

void NavigationData::InitCells()
{
	memset(m_cells, PER_NON_CELL, PER_MAX_CELL * PER_MAX_CELL * sizeof(int));
}

void NavigationData::SetCells(std::vector<PERObject*>& objects, int numObject)
{
	for (int i = 0; i < numObject; ++i) {
		if (objects[i]->GetObjectType() != PERObjectType::FIXED_BLOCK) continue;
		
		PERVec3 pos = objects[i]->GetPosition();
		PERVec3 size = objects[i]->GetSize();

		int indexX, indexY;
		int zValue = (int)(floor(pos.z) + PER_CELL_DATA_WEIGHT);

		for (double x = pos.x - size.x / 2; x < pos.x + size.x / 2 + PER_CELL_DISTANCE; x += PER_CELL_DISTANCE) {
			for (double y = pos.y - size.y / 2; y < pos.y + size.y / 2 + PER_CELL_DISTANCE; y += PER_CELL_DISTANCE) {
				if (x < 0.0) indexX = (PER_MAX_CELL / 2) + (int)(x / PER_CELL_DISTANCE);
				else indexX = (int)(x / PER_CELL_DISTANCE) + (PER_MAX_CELL / 2);
					
				if (y < 0.0) indexY = (PER_MAX_CELL / 2) + (int)(y / PER_CELL_DISTANCE);
				else indexY = (int)(y / PER_CELL_DISTANCE) + (PER_MAX_CELL / 2);

				if (m_cells[indexX][indexY] < zValue)
					m_cells[indexX][indexY] = zValue;
					
			}
		}
	}
}

void NavigationData::TextOutDataForDebug()
{
	std::ofstream out("nvData.txt");

	for (int y = 0; y < PER_MAX_CELL; ++y) {
		for (int x = 0; x < PER_MAX_CELL; ++x) {
			out << (int)m_cells[x][y];
		}
		out << '\n';
	}

	out.close();
}

void NavigationData::MakeDataToFile(const char* fileName)
{
	PERLog::Logger().Info("네비게이션 데이터 출력");

	std::ofstream out(fileName, std::ios::binary);

	out.write(reinterpret_cast<const char*>(m_cells), sizeof(int) * PER_MAX_CELL * PER_MAX_CELL);

	out.close();
}

void NavigationData::ReadDataFromFile(const char* fileName)
{
	PERLog::Logger().Info("네비게이션 데이터 입력");

	std::ifstream in(fileName, std::ios::binary);

	in.read(reinterpret_cast<char*>(m_cells), sizeof(int) * PER_MAX_CELL * PER_MAX_CELL);

	in.close();
}

void NavigationData::RenderOutData(PERRenderer& renderer)
{
	PERColor color;
	for (int x = 0; x < PER_MAX_CELL; ++x) {
		for (int y = 0; y < PER_MAX_CELL; ++y) {
			if (GetCellInfo(x, y) == INT_MIN) continue; 
		
			color = PERColor(128 + GetCellInfo(x, y) * 10, 128 + GetCellInfo(x, y) * 10, 128);

			PERVec3 position = ChangeCellToWorldPosition(x, y);
			renderer.RenderShapeInWorldCoordinate(PERShapeType::ELLIPSE, 
			PERVec3(position.x, position.y, 2.0),
			PERVec3(0.1, 0.1, 1.0), color);
		}
	}
}

int NavigationData::GetCellInfo(int x, int y) const
{
	return m_cells[x][y];
}

inline PERVec3 NavigationData::ChangeCellToWorldPosition(int cellX, int cellY)
{
	return PERVec3((cellX - PER_MAX_CELL / 2) * PER_CELL_DISTANCE, (cellY - PER_MAX_CELL / 2) * PER_CELL_DISTANCE, -1.0);
}
