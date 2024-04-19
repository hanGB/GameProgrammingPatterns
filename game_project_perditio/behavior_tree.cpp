#include "stdafx.h"
#include "behavior_tree.h"


BehaviorTree::BehaviorTree(std::string name, BTNode* root)
	: m_name(name), m_root(root)
{
	
}

BehaviorTree::~BehaviorTree()
{
	delete m_root;
}

void BehaviorTree::Run(PERObject& object, double dTime)
{
	m_root->Run(object, dTime);
}


// -------------------------------------- Node -------------------------------------- //


SequencerNode::SequencerNode(std::string name)
{
	m_name = name;
}

SequencerNode::~SequencerNode()
{
	for (auto& child : m_children) delete child;
}

PERBehaviorResult SequencerNode::Run(PERObject& object, double dTime)
{
	for (size_t i = m_current; i < m_children.size(); ++i) {
		PERBehaviorResult result = m_children[i]->Run(object, dTime);

		if (result == PERBehaviorResult::FAIL) {
			m_current = 0;
			return result;
		}
		else if (result == PERBehaviorResult::RUNNING) {
			m_current = i;
			return result;
		}
	}
	m_current = 0;
	return PERBehaviorResult::SUCCESS;
}

void SequencerNode::AddChild(BTNode* node)
{
	m_children.push_back(node);
}

SelectorNode::SelectorNode(std::string name)
{
	m_name = name;
}

SelectorNode::~SelectorNode()
{
	for (auto& child : m_children) delete child;
}

PERBehaviorResult SelectorNode::Run(PERObject& object, double dTime)
{
	for (size_t i = m_current; i < m_children.size(); ++i) {
		PERBehaviorResult result = m_children[i]->Run(object, dTime);

		if (result == PERBehaviorResult::SUCCESS) {
			m_current = 0;
			return result;
		}
		else if (result == PERBehaviorResult::RUNNING) {
			m_current = i;
			return result;
		}
	}
	m_current = 0;
	return PERBehaviorResult::FAIL;
}

void SelectorNode::AddChild(BTNode* node)
{
	m_children.push_back(node);
}
