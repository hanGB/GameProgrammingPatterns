#pragma once

class PERObject;

enum class NavigationNodeType {
	NOTHING,
	GROUND,
	WALL
};

class NavigationData {
public:
	NavigationData();
	~NavigationData();

	void SetNodes(std::vector<PERObject*>& objects, int numObject);
	// 테스트용 텍스트 파일로 출력
	void TextOutData();

private:
	static const int c_MAX_NODE = 100;
	const double c_NODE_DISTANCE = 0.25;

	NavigationNodeType m_nodes[c_MAX_NODE][c_MAX_NODE];
};