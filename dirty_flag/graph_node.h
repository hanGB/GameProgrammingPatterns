#pragma once
#include "stdafx.h"
#include "mesh.h"
#include "transform.h"

class GraphNode {
public:
	GraphNode(Mesh* mesh) : m_mesh(mesh), m_local(Transform::Origin()) {

	}
	GraphNode(Mesh* mesh, Transform local) : m_mesh(mesh), m_local(local) {

	}
	~GraphNode() {
		for (int i = 0; i < m_numChildren; ++i) {
			delete m_children[i];
		}
	}

	void Render(Transform parentWorld) {
		Transform world = m_local.Combine(parentWorld);
		if (m_mesh) RenderMesh(m_mesh, world);

		for (int i = 0; i < m_numChildren; ++i) {
			m_children[i]->Render(world);
		}
	}
	void AddChild(GraphNode* graphNode) {
		assert(m_numChildren < MAX_GRAPH_NODE_CHILDREN);

		m_children[m_numChildren] = graphNode;
		m_numChildren++;
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
};