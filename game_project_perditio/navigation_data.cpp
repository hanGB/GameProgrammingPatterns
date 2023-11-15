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
	memset(m_cells, (int)NavigationCellType::NOTHING, PER_MAX_CELL * PER_MAX_CELL * sizeof(NavigationCellType));
}

void NavigationData::SetCells(std::vector<PERObject*>& objects, int numObject)
{
	for (int i = 0; i < numObject; ++i) {
		if (objects[i]->GetObjectType() != PERObjectType::FIXED_BLOCK) continue;
		
		PERVec3 pos = objects[i]->GetPosition();
		PERVec3 size = objects[i]->GetSize();

		int indexX, indexY;

		switch ((int)pos.z) {
		case PER_PLATFORM_Z_VALUE:
		{
			// 땅 취급
			for (double x = pos.x - size.x / 2; x < pos.x + size.x / 2 + PER_CELL_DISTANCE; x += PER_CELL_DISTANCE) {
				for (double y = pos.y - size.y / 2; y < pos.y + size.y / 2 + PER_CELL_DISTANCE; y += PER_CELL_DISTANCE) {
					if (x < 0.0) indexX = (PER_MAX_CELL / 2) + (int)(x / PER_CELL_DISTANCE);
					else indexX = (int)(x / PER_CELL_DISTANCE) + (PER_MAX_CELL / 2);
					
					if (y < 0.0) indexY = (PER_MAX_CELL / 2) + (int)(y / PER_CELL_DISTANCE);
					else indexY = (int)(y / PER_CELL_DISTANCE) + (PER_MAX_CELL / 2);

					if (m_cells[indexX][indexY] != NavigationCellType::WALL)
						m_cells[indexX][indexY] = NavigationCellType::GROUND;
					
				}
			}

		}
		break;
		case PER_NORAML_OBJECT_Z_VALUE:
		{
			// 벽 취급
			for (double x = pos.x - size.x / 2; x < pos.x + size.x / 2 + PER_CELL_DISTANCE; x += PER_CELL_DISTANCE) {
				for (double y = pos.y - size.y / 2; y < pos.y + size.y / 2 + PER_CELL_DISTANCE; y += PER_CELL_DISTANCE) {
					if (x < 0.0) indexX = (PER_MAX_CELL / 2) + (int)(x / PER_CELL_DISTANCE);
					else indexX = (int)(x / PER_CELL_DISTANCE) + (PER_MAX_CELL / 2);

					if (y < 0.0) indexY = (PER_MAX_CELL / 2) + (int)(y / PER_CELL_DISTANCE);
					else indexY = (int)(y / PER_CELL_DISTANCE) + (PER_MAX_CELL / 2);

					m_cells[indexX][indexY] = NavigationCellType::WALL;
				}
			}
		}
		break;
		case PER_ROOF_Z_VALUE:
		{
			// 위에 있는 물체로 아무것도 안함
		}
		break;
		}
	}
}

void NavigationData::TextOutData()
{
	std::ofstream out;

	out.open("NavData.txt");

	for (int y = 0; y < PER_MAX_CELL; ++y) {
		for (int x = 0; x < PER_MAX_CELL; ++x) {
			out << (int)m_cells[x][y];
		}
		out << '\n';
	}

	out.close();
}

void NavigationData::RenderOutData(PERRenderer& renderer)
{
	PERColor color;
	for (int x = 0; x < PER_MAX_CELL; ++x) {
		for (int y = 0; y < PER_MAX_CELL; ++y) {
			if (GetCellInfo(x, y) == NavigationCellType::GROUND) color = PERColor(255, 255, 0);
			else if (GetCellInfo(x, y) == NavigationCellType::WALL) color = PERColor(255, 0, 255);
			else continue;

		PERVec3 position = ChangeCellToWorldPosition(x, y);
		renderer.RenderShapeInWorldCoordinate(PERShapeType::ELLIPSE, 
		PERVec3(position.x, position.y, 2.0),
		PERVec3(0.1, 0.1, 1.0), color);
		}
	}
}

NavigationCellType NavigationData::GetCellInfo(int x, int y) const
{
	return m_cells[x][y];
}

inline PERVec3 NavigationData::ChangeCellToWorldPosition(int cellX, int cellY)
{
	return PERVec3((cellX - PER_MAX_CELL / 2) * PER_CELL_DISTANCE, (cellY - PER_MAX_CELL / 2) * PER_CELL_DISTANCE, -1.0);
}
