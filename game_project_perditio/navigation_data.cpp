#include "stdafx.h"
#include "navigation_data.h"
#include "per_object.h"

NavigationData::NavigationData()
{
	memset(m_nodes, (int)NavigationNodeType::NOTHING, c_MAX_NODE * c_MAX_NODE * sizeof(NavigationNodeType));
}

NavigationData::~NavigationData()
{
}

void NavigationData::SetNodes(std::vector<PERObject*>& objects, int numObject)
{
	for (int i = 0; i < numObject; ++i) {
		if (objects[i]->GetObjectType() != PERObjectType::FIXED_BLOCK) continue;
		
		PERVec3 pos = objects[i]->GetPosition();
		PERVec3 size = objects[i]->GetSize();

		int indexX, indexY;

		switch ((int)pos.z) {
		case -1:
		{
			// 땅 취급
			for (double x = pos.x - size.x / 2; x < pos.x + size.x / 2; x += c_NODE_DISTANCE) {
				for (double y = pos.y - size.y / 2; y < pos.y + size.y / 2; y += c_NODE_DISTANCE) {
					if (x < 0.0) indexX = (c_MAX_NODE / 2) + (int)(x / c_NODE_DISTANCE);
					else indexX = (int)(x / c_NODE_DISTANCE) + (c_MAX_NODE / 2);
					
					if (y < 0.0) indexY = (c_MAX_NODE / 2) + (int)(y / c_NODE_DISTANCE);
					else indexY = (int)(y / c_NODE_DISTANCE) + (c_MAX_NODE / 2);

					if (m_nodes[indexX][indexY] != NavigationNodeType::WALL)
						m_nodes[indexX][indexY] = NavigationNodeType::GROUND;
					
				}
			}

		}
		break;
		case 0:
		{
			// 벽 취급
			for (double x = pos.x - size.x / 2; x < pos.x + size.x / 2; x += c_NODE_DISTANCE) {
				for (double y = pos.y - size.y / 2; y < pos.y + size.y / 2; y += c_NODE_DISTANCE) {
					if (x < 0.0) indexX = (c_MAX_NODE / 2) + (int)(x / c_NODE_DISTANCE);
					else indexX = (int)(x / c_NODE_DISTANCE) + (c_MAX_NODE / 2);

					if (y < 0.0) indexY = (c_MAX_NODE / 2) + (int)(y / c_NODE_DISTANCE);
					else indexY = (int)(y / c_NODE_DISTANCE) + (c_MAX_NODE / 2);

					m_nodes[indexX][indexY] = NavigationNodeType::WALL;
				}
			}

		}
		break;
		case 1:
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

	for (int y = 0; y < c_MAX_NODE; ++y) {
		for (int x = 0; x < c_MAX_NODE; ++x) {
			out << (int)m_nodes[x][y];
		}
		out << '\n';
	}

	out.close();
}
