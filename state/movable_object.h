#pragma once
#include "gpp_game_object.h"

class Player : public GPPGameObject {
public:
	Player(float positionX, float positionY) {

		SetPosition(positionX, positionY);
		SetHalfSize(0.4f, 0.4f);
		SetRGBColor(0, 127, 127);
		SetMass(50.0f);
		m_state = GPPGameObjectState::STANDING_ON_GROUND;
	}
	~Player() {}

	virtual void Update(float elapsedTime, GPPInputChunk& inputs) override {
		GiveForceWithInput(elapsedTime, inputs);

		if (fabs(m_velocityX) > 0.f && m_state == GPPGameObjectState::STANDING_ON_GROUND) {
			// ¸¶Âû
			m_currentAccX += m_velocityX / std::abs(m_velocityX) * GPP_FRICTION * (-GPP_GRAVITY);

			float tempVelocityX = m_velocityX + m_currentAccX * elapsedTime;
			if (tempVelocityX * m_velocityX < 0.0f) {
				m_velocityX = 0.0f;
			}
			else {
				m_velocityX = tempVelocityX;
			}

			m_velocityX += m_currentAccX * elapsedTime;
			m_velocityY = 0.0f;
		}
		else if (m_state == GPPGameObjectState::FALLING) {
			m_currentAccY -= GPP_GRAVITY;
			m_velocityY += m_currentAccY * elapsedTime;
		}
		else {
			m_velocityX += m_currentAccX * elapsedTime;
			m_velocityY = 0.0f;

		}

		m_positionX += (m_velocityX * elapsedTime + 0.5f * m_currentAccX * elapsedTime * elapsedTime);
		m_positionY += (m_velocityY * elapsedTime + 0.5f * m_currentAccY * elapsedTime * elapsedTime);
	}
	virtual void Render(HDC& memDC, float posInWindowX, float posInWindowY) override {

		Ellipse(memDC,
			static_cast<int>((posInWindowX - m_halfSizeX) * GPP_PIXEL_PER_METER),
			static_cast<int>((posInWindowY - m_halfSizeY) * GPP_PIXEL_PER_METER),
			static_cast<int>((posInWindowX + m_halfSizeX) * GPP_PIXEL_PER_METER),
			static_cast<int>((posInWindowY + m_halfSizeY) * GPP_PIXEL_PER_METER)
		);
	}

private:
	void GiveForceWithInput(float elapsedTime, GPPInputChunk& inputs) {

		m_currentAccX = 0.0f;
		m_currentAccY = 0.0f;

		if (m_state == GPPGameObjectState::STANDING_ON_GROUND) {
			if (inputs.Input_A)
				if (m_velocityX > -MAXIUM_VELOCITY_X) m_currentAccX -= FORCE_X / m_mass;

			if (inputs.Input_D)
				if (m_velocityX < MAXIUM_VELOCITY_X) m_currentAccX += FORCE_X / m_mass;
		}
	}

	const float FORCE_X = 500.0f;
	const float FORCE_Y = 100.0f;

	const float MAXIUM_VELOCITY_X = 3.0f;

	float m_currentAccX, m_currentAccY;
};
