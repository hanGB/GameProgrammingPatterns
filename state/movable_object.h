#pragma once
#include "gpp_game_object.h"

class MovableObject : public GPPGameObject {
public:
	MovableObject() {
		m_currentAccX = 0.0f;
		m_currentAccY = 0.0f;
	}
	MovableObject(float positionX, float positionY, float sizeX, float sizeY, int red, int green, int blue) {
		
		SetPosition(positionX, positionY);
		SetHalfSize(sizeX / 2.0f, sizeY / 2.0f);
		SetRGBColor(red, green, blue);
		SetIsFalling(false);
		m_currentAccX = 0.0f;
		m_currentAccY = 0.0f;
	}

	~MovableObject() {}

	virtual void Update(float elapsedTime) override {

		// 이동 관련
		if (fabs(m_velocityX) > 0.f && !GetIsFalling()) {
			// 마찰
			m_currentAccX += m_velocityX / std::abs(m_velocityX) * GPP_FRICTION * (-GPP_GRAVITY);

			float tempVelocityX = m_velocityX + m_currentAccX * elapsedTime;
			if (tempVelocityX * m_velocityX < 0.0f) {
				m_velocityX = 0.0f;
			}
			else {
				m_velocityX = tempVelocityX;
			}

			m_velocityX += m_currentAccX * elapsedTime;
		}
		else if (GetIsFalling()) {
			m_currentAccY -= GPP_GRAVITY;
			m_velocityY += m_currentAccY * elapsedTime;
		}
		else {
			m_velocityX += m_currentAccX * elapsedTime;
		}

		m_positionX += (m_velocityX * elapsedTime + 0.5f * m_currentAccX * elapsedTime * elapsedTime);
		m_positionY += (m_velocityY * elapsedTime + 0.5f * m_currentAccY * elapsedTime * elapsedTime);

		// 현재 가속도 초기화
		m_currentAccX = 0.0f;
		m_currentAccY = 0.0f;
	}
	virtual void Render(HDC& memDC, float posInWindowX, float posInWindowY) override {

		Ellipse(memDC,
			static_cast<int>((posInWindowX - m_halfSizeX) * GPP_PIXEL_PER_METER),
			static_cast<int>((posInWindowY - m_halfSizeY) * GPP_PIXEL_PER_METER),
			static_cast<int>((posInWindowX + m_halfSizeX) * GPP_PIXEL_PER_METER),
			static_cast<int>((posInWindowY + m_halfSizeY) * GPP_PIXEL_PER_METER)
		);
	}

	void GiveForce(float amountX, float amountY) {
		m_currentAccX = amountX / m_mass;
		m_currentAccY = amountY / m_mass;
	}

	void GetCurrentAccel(float* x, float* y) {
		*x = m_currentAccX;
		*y = m_currentAccY;
	}
	void SetCurrentAccel(float x, float y) {
		m_currentAccX = x;
		m_currentAccY = y;
	}

protected:
	float m_currentAccX, m_currentAccY;
};
