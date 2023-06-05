#pragma once
#include "stdafx.h"
#include "mesh.h"
#include "transform.h"

class GraphNode {
public:
	GraphNode(Mesh* mesh) : m_mesh(mesh), m_local(Transform::Origin()), m_dirty(true) {

	}
	GraphNode(Mesh* mesh, Transform local) : m_mesh(mesh), m_local(local) {

	}
	~GraphNode() {

	}

	void Render(Transform parentWorld, bool dirty) {
		// or 연산 -> dirty와 m_dirty 둘 중 하나만 true면 true;
		dirty |= m_dirty;

		if (dirty) {
			m_world = m_local.Combine(parentWorld);
			m_dirty = false;
		}

		if (m_mesh) RenderMesh(m_mesh, m_world);

		for (int i = 0; i < m_numChildren; ++i) {
			m_children[i]->Render(m_world, dirty);
		}
	}
	void AddChild(GraphNode* graphNode) {
		assert(m_numChildren < MAX_GRAPH_NODE_CHILDREN);

		m_children[m_numChildren] = graphNode;
		m_numChildren++;
	}
	void SetTransform(Transform local) {
		m_local = local;
		m_dirty = true;
	}

private:
	void RenderMesh(Mesh* mesh, Transform world)
	{
		glm::vec4 vertex = glm::vec4(mesh->GetVertex(), 1.0f);

		glm::vec4 vertexInWorld = world.GetMatrix() * vertex;

		std::cout << vertexInWorld.x << ", " << vertexInWorld.y << ", " << vertexInWorld.z << std::endl;
	}

	Transform	m_local;
	Mesh*		m_mesh;
	GraphNode*	m_children[MAX_GRAPH_NODE_CHILDREN];
	int			m_numChildren = 0;

	// 더티 플래그
	Transform	m_world;
	bool		m_dirty;
};