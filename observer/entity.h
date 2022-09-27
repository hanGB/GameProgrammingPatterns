#pragma once
#include "stdafx.h"

class Entity {
public:
	Entity() {
		m_positionX = 0.0f;
		m_positionY = 0.0f;
		m_speedX = 20.0f;
		m_speedY = 0.0f;
	}
	~Entity() {}


	bool GetIsHero() const {
		return m_isHero;
	}
	void GetPosition(float* x, float* y) {
		*x = m_positionX;
		*y = m_positionY;
	}
	bool GetIsOnSurface() const {
		return m_IsOnSurface;
	}

	void SetIsHero(bool hero) {
		m_isHero = hero;
	}
	void SetPosition(float x, float y) {
		m_positionX = x;
		m_positionY = y;
	}
	void SetAcelerate(float x, float y) {
		m_acelerateX = x;
		m_acelerateY = y;
	}

	void MoveWithAxisX(int dir, float elapsedTime) {
		m_positionX += dir * elapsedTime * m_speedX;
	}

	void Update(float elapsedTime) {
		if (m_IsOnSurface) {
			m_acelerateY = 0.0f;
			m_speedY = 0.0f;
		}

		if (m_positionX > 50.0f || m_positionX < -50.0f) {
			m_IsOnSurface = false;
		}

		float t = elapsedTime;
		m_positionY = m_positionY + m_speedY * t + 0.5f * m_acelerateY * t * t;
		m_speedY = m_speedY + m_acelerateY * t;
	}
	void Render() {
		std::cout << m_positionX << ", " << m_positionY << '\n';
	}

private:
	bool m_isHero = false;
	float m_positionX, m_positionY;
	float m_speedX, m_speedY;
	float m_acelerateX, m_acelerateY;

	bool m_IsOnSurface = true;
};