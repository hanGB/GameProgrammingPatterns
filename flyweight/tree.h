#pragma once
#include "stdafx.h"

class TreeModel {
public:
	TreeModel(int mesh, int bark, int leaves) : m_mesh(mesh), m_barkTexture(bark), m_leavesTexture(leaves) {};
	~TreeModel() {};

	void GetData(int* mesh, int* bark, int* leaves) {
		*mesh = m_mesh;
		*bark = m_barkTexture;
		*leaves = m_leavesTexture;
	}
	void GetData2(int& mesh, int& bark, int& leaves) {
		mesh = m_mesh;
		bark = m_barkTexture;
		leaves = m_leavesTexture;
	}

private:
	int m_mesh;
	int m_barkTexture;
	int m_leavesTexture;
};

class Tree {
public:
	Tree(int id, TreeModel* model) : m_id(id), m_model(model) {};
	~Tree() {};

	void Render() {
		std::cout << m_id << ": "
			<< m_position[0] << ", "
			<< m_position[1] << ", "
			<< m_position[2] << '\n';

		int mesh, bark, leaves;

		m_model->GetData(&mesh, &bark, &leaves);
		std::cout << mesh << ", " << bark << ", " << leaves << '\n';
		m_model->GetData2(mesh, bark, leaves);
		std::cout << mesh << ", " << bark << ", " << leaves << '\n';
	}

	void SetPosition(float x, float y, float z) {
		m_position[0] = x;
		m_position[1] = y;
		m_position[2] = z;
	}

private:
	TreeModel* m_model;

	int m_id;
	float m_position[3];
	double m_height = 1.0f;
	double m_thickness = 1.0f;
	float m_barkTint[3] = { 1.0f, 1.0f, 1.0f };
	float m_leafTint[3] = { 1.0f, 1.0f, 1.0f };
};