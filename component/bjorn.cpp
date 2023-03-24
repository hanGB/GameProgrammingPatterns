#include "stdafx.h"
#include "bjorn.h"
#include "comp_controller.h"

Bjorn::Bjorn()
{
	m_posX = 0.0; m_posY = 0.0;
	m_sizeX = 0.8; m_sizeY = 0.8f;
	m_velocityX = 0.0; m_velocityY = 0.0;
	m_mass = 5.0;
	m_isFalling = true;

	m_idleColor.red = 0; m_idleColor.green = 127; m_idleColor.blue = 127;
	m_walkRightColor.red = 0; m_walkRightColor.green = 0; m_walkRightColor.blue = 127;
	m_walkLeftColor.red = 0; m_walkLeftColor.green = 127; m_walkLeftColor.blue = 0;

	m_color = m_idleColor;
}

Bjorn::~Bjorn()
{
}

void Bjorn::Update(CompWorld& world, double elapsedTimeInSec)
{
	double accX = 0.0, accY = 0.0;

	if (CompController::GetInstance()->GetXAxisDirection() < 0)
	{
		if (m_velocityX > -MAXIUM_VELOCITY_X && !m_isFalling) {
			accX -= FORCE_X / m_mass;
		}
	}
	else if (CompController::GetInstance()->GetXAxisDirection() > 0)
	{
		if (m_velocityX < MAXIUM_VELOCITY_X && !m_isFalling) {
			accX += FORCE_X / m_mass;
		}
	}

	double t = elapsedTimeInSec;

	// 이동 관련
	if (fabs(m_velocityX) > 0.f && !m_isFalling) {
		// 마찰
		accX += m_velocityX / std::abs(m_velocityX) * GPP_FRICTION * (-GPP_GRAVITY);

		double tempVelocityX = m_velocityX + accX * t;
		if (tempVelocityX * m_velocityX < 0.0f) {
			m_velocityX = 0.0f;
		}
		else {
			m_velocityX = tempVelocityX;
		}

		m_velocityX += accX * t;
	}
	else if (m_isFalling) {
		accY -= GPP_GRAVITY;
		m_velocityY += accY * t;
	}
	else {
		m_velocityX += accX * t;
	}

	m_posX += (m_velocityX * t + 0.5f * accX * t * t);
	m_posY += (m_velocityY * t + 0.5f * accY * t * t);

	m_isFalling = !world.CheckCollision(&m_posX, &m_posY, m_sizeX, m_sizeY);

	if (m_velocityX > STOP_VELOCITY) {
		m_color = m_walkRightColor;
	}
	else if (m_velocityX < -STOP_VELOCITY) {
		m_color = m_walkLeftColor;
	}
	else {
		m_color = m_idleColor;
	}

}

void Bjorn::Render(CompRenderer& renderer)
{
	renderer.RenderEllipse(m_posX, m_posY, m_sizeX, m_sizeY, m_color);
}
