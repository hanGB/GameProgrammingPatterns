#pragma once
#include "stdafx.h"

class Mesh {
public:
	Mesh() : m_vertex(glm::vec3(0.0f, 0.0f, 0.0f)) {}
	Mesh(glm::vec3 vertex) : m_vertex(vertex) {}
	~Mesh() {}

	glm::vec3 GetVertex() const {
		return m_vertex;
	}
	void SetVertex(glm::vec3 vertex) {
		m_vertex = vertex;
	}

private:
	glm::vec3 m_vertex;
};