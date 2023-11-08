#pragma once

class PERObject;
class PERRenderer;

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
	// 테스트용 화면에 표시
	void RenderOutData(PERRenderer& renderer);

	NavigationCellType GetCellInfo(int x, int y) const;

private:
	NavigationCellType m_cells[PER_MAX_CELL][PER_MAX_CELL];
};