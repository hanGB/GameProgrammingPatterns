#pragma once

class PERObject;

enum class NavigationCellType {
	NOTHING,
	GROUND,
	WALL
};

class NavigationData {
public:
	NavigationData();
	~NavigationData();

	void InitCells();
	void SetCells(std::vector<PERObject*>& objects, int numObject);
	// 테스트용 텍스트 파일로 출력
	void TextOutData();

	NavigationCellType GetCellInfo(int x, int y) const;

private:
	NavigationCellType m_cells[PER_MAX_CELL][PER_MAX_CELL];
};