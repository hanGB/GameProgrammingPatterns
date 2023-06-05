#pragma once
#include "stdafx.h"

class Transform {
public:
	static Transform Origin()
	{
		Transform transform = Transform();
		glm::mat4 matrix = transform.GetMatrix();
		matrix = glm::mat4(1.0);
		transform.SetMatrix(matrix);

		return transform;
	}
	Transform Combine(Transform& other)
	{
		std::cout << "----- 행렬 곱 연산 실행 -----\n";
		Transform transform = Transform();
		transform.SetMatrix(other.GetMatrix() * m_matrix);

		return transform;
	}

	glm::mat4 GetMatrix() const
	{
		return m_matrix;
	}
	void SetMatrix(glm::mat4 matrix)
	{
		m_matrix = matrix;
	}

private:
	glm::mat4 m_matrix;
};