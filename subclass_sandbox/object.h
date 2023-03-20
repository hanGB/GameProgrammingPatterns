#pragma once
#include "stdafx.h"

class Object {
public:
	Object() : m_x(0.0), m_y(0.0), m_z(0.0) { }
	~Object() { }

	void GetPosition(double* x, double* y, double* z) {
		*x = m_x;
		*y = m_y;
		*z = m_z;
	}
	void SetPosition(double x, double y, double z) {
		m_x = x;
		m_y = y;
		m_z = z;
	}
	virtual void Render() {
		std::cout << m_x << ", " << m_y << ", " << m_z;
	}
	virtual void Update() {
		
	}

private:
	double m_x, m_y, m_z;
};