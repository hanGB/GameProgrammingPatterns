#pragma once
#include "gpp_game_object.h"

class MovableObject : public GPPGameObject {
public:
	MovableObject() {

		m_state = GPPGameObjectState::STANDING;
		m_currentAccX = 0.0f;
		m_currentAccY = 0.0f;
	}
	~MovableObject() {}

	virtual void Update(float elapsedTime) override {

		// 이동 관련
		if (fabs(m_velocityX) > 0.f && (m_state == GPPGameObjectState::STANDING || m_state == GPPGameObjectState::DUCKING)) {
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
		else if (m_state == GPPGameObjectState::FALLING || m_state == GPPGameObjectState::DIVING) {
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

protected:
	float m_currentAccX, m_currentAccY;
};

class Player : public MovableObject {
public:
	Player(float positionX, float positionY) {

		SetPosition(positionX, positionY);
		SetHalfSize(0.4f, 0.4f);
		SetRGBColor(0, 127, 127);
		SetMass(50.0f);
	}
	~Player() {}

	virtual void HandleInput(GPPInputChunk& inputs) override {
		switch (m_state)
		{
		case GPPGameObjectState::STANDING: 
		{
			if (inputs.Input_A) 
			{
				if (m_velocityX > -MAXIUM_VELOCITY_X) m_currentAccX -= FORCE_X / m_mass;
			}
			if (inputs.Input_D)
			{
				if (m_velocityX < MAXIUM_VELOCITY_X) m_currentAccX += FORCE_X / m_mass;
			}

			if (inputs.Input_Space)
			{
				m_currentAccY = FORCE_Y / m_mass;
				m_state = GPPGameObjectState::FALLING;
			}
			else if (inputs.Input_S)
			{
				m_halfSizeY /= 2.0f;
				m_positionY -= m_halfSizeY;
				m_chargeTime = 0.0f;
				m_rgbColor[0] = 0;
				m_state = GPPGameObjectState::DUCKING;
			}
			break;
		}
		case GPPGameObjectState::FALLING:
		{	
			if (inputs.Input_S) 
			{
				m_velocityY = -10.0f;
				m_state = GPPGameObjectState::DIVING;
			}
			break;
		}
		case GPPGameObjectState::DUCKING: 
		{
			if (inputs.Input_A)
			{
				if (m_velocityX > -MAXIUM_VELOCITY_X) m_currentAccX -= FORCE_X / m_mass;
			}
			if (inputs.Input_D)
			{
				if (m_velocityX < MAXIUM_VELOCITY_X) m_currentAccX += FORCE_X / m_mass;
			}
			if (!inputs.Input_S) 
			{
				m_halfSizeY *= 2.0f;
				m_chargeTime = 0.0f;
				m_rgbColor[0] = 0;
				m_state = GPPGameObjectState::FALLING;
			}
			break;
		}
		}
	}

	virtual void Update(float elapsedTime) override {

		MovableObject::Update(elapsedTime);

		// 나머지
		if (m_state == GPPGameObjectState::DUCKING) {
			m_chargeTime += elapsedTime;
			
			m_rgbColor[0] = static_cast<int>(MAXIMUM_RGB_BRIGHTNESS * m_chargeTime / MAX_CHARGE);

			if (MAX_CHARGE < m_chargeTime) {
				m_chargeTime = 0.0f;
				m_rgbColor[0] = 0;
			}
		}
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
	const float FORCE_X = 500.0f;
	const float FORCE_Y = 10000.0f;

	const float MAXIUM_VELOCITY_X = 3.0f;

	const float MAX_CHARGE = 1.0f;

	float m_chargeTime;
};
