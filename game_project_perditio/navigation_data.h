#pragma once

class PERObject;
class PERRenderer;

class NavigationData {
public:
	NavigationData();
	~NavigationData();

	void InitCells();
	void SetCells(std::vector<PERObject*>& objects, int numObject);
	// �׽�Ʈ�� �ؽ�Ʈ ���Ϸ� ���
	void TextOutData();
	// �׽�Ʈ�� ȭ�鿡 ǥ��
	void RenderOutData(PERRenderer& renderer);

	int GetCellInfo(int x, int y) const;
	// ���� ���� ��ǥ�� ����
	PERVec3 ChangeCellToWorldPosition(int cellX, int cellY);

private:
	int m_cells[PER_MAX_CELL][PER_MAX_CELL];
};