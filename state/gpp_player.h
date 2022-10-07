#pragma once
#include "movable_object.h"
#include "gpp_spawner.h"


class GPPPlayer : public MovableObject, public GPPSpawner {
public:
	GPPPlayer(float positionX, float positionY, float sizeX, float sizeY, int red, int green, int blue) {
		SetPosition(positionX, positionY);
		SetHalfSize(sizeX / 2.0f, sizeY / 2.0f);
		SetRGBColor(red, green, blue);

		SetMass(50.0f);
	}
	~GPPPlayer() {}

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
				m_state = GPPGameObjectState::STANDING;
			}
			break;
		}
		}
	}

	virtual void Update(float elapsedTime) override {

		MovableObject::Update(elapsedTime);

		if (m_state == GPPGameObjectState::DUCKING) {
			m_chargeTime += elapsedTime;

			m_rgbColor[0] = static_cast<int>(MAXIMUM_RGB_BRIGHTNESS * m_chargeTime / MAX_CHARGE);

			if (MAX_CHARGE < m_chargeTime) {
				ShotBomb();
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
	void ShotBomb() {
		m_chargeTime = 0.0f;
		m_rgbColor[0] = 0;
		MovableObject* bomb = GetGPPWorld()->SpawnObject<MovableObject>(m_positionX, m_positionY + 0.25f, 0.4f, 0.4f, 0, 0, 0);
		bomb->SetMass(10.0f);
		bomb->GiveForce(5000.0f, 0.0f);
		bomb->SetLifeTime(3.0f);
	}

	const float FORCE_X = 500.0f;
	const float FORCE_Y = 10000.0f;

	const float MAXIUM_VELOCITY_X = 3.0f;

	const float MAX_CHARGE = 1.0f;

	float m_chargeTime;

};
