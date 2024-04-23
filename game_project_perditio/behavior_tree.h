#pragma once

class AiComponent;
class PERObject;
class BTNode;

enum class PERBehaviorResult {
	SUCCESS,
	FAIL,
	RUNNING
};

class BehaviorTree {
public:
	BehaviorTree(std::string name, BTNode* root);
	~BehaviorTree();

	void Run(PERObject& object, double dTime);

private:
	std::string m_name;
	BTNode* m_root;
};

class BTNode {
public:
	virtual PERBehaviorResult Run(PERObject& object, double dTime) = 0;

protected:
	std::string m_name;
};

class SequencerNode : public BTNode {
public:
	SequencerNode(std::string name);
	~SequencerNode();

	virtual PERBehaviorResult Run(PERObject& object, double dTime);

	void AddChild(BTNode* node);

private:
	std::vector<BTNode*> m_children;
	size_t m_current = 0;
};

class SelectorNode : public BTNode {
public:
	SelectorNode(std::string name);
	~SelectorNode();

	virtual PERBehaviorResult Run(PERObject& object, double dTime);

	void AddChild(BTNode* node);

private:
	std::vector<BTNode*> m_children;
	size_t m_current = 0;
};

// 각 컨포넌트 클래스 별로 생성되어 사용됨
template <class TComponent>
class LeafNode : public BTNode {
public:
	LeafNode(std::string name, std::function<PERBehaviorResult(TComponent&, PERObject&, double dTime)> function, TComponent& m_component)
		: m_function(function), m_component(m_component)
	{
		m_name = name;
	}

	virtual PERBehaviorResult Run(PERObject& object, double dTime)
	{
		return m_function(m_component, object, dTime);
	}


private:
	std::function<PERBehaviorResult(TComponent&, PERObject&, double dTime)> m_function;
	TComponent& m_component;
};