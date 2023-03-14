#pragma once
#include "stdafx.h"
#include "renderer.h"

class Entity {
public:
	Entity() : m_x(0), m_y(0), m_model('?'), m_isDead(false) {}
	virtual ~Entity() {}
	virtual void Update(double elapsedInSec) = 0;
	virtual void Render(Renderer* renderer) 
	{
		char temp[5];
		sprintf_s(temp, "%c", m_model);
		renderer->PrintOnBuffer(m_x, m_y, temp);
	}

	double GetX() const { return m_x; }
	double GetY() const { return m_y; }
	char GetModel() const { return m_model; }

	bool GetIsDead() const { return m_isDead; }

	void SetX(double x) { m_x = x; }
	void SetY(double y) { m_y = y; }
	void SetModel(char model) { m_model = model; }

	void SetIsDead(bool isDead) { m_isDead = isDead; }

private:
	double	m_x; 
	double	m_y; 
	char	m_model;

	bool m_isDead;
};