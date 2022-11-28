#include "gpp_player.h"
#include "player_state.h"
#include "gpp_world.h"

GPPPlayer::GPPPlayer(float positionX, float positionY, float sizeX, float sizeY, int red, int green, int blue)
{
	SetPosition(positionX, positionY);
	SetHalfSize(sizeX / 2.0f, sizeY / 2.0f);
	SetRGBColor(red, green, blue);

	SetMass(50.0f);
	m_state = new FallingState();
}

GPPPlayer::~GPPPlayer()
{
}

void GPPPlayer::HandleInput(GPPInputChunk& inputs)
{
	PlayerState* state = m_state->HandleInput(*this, inputs);
	if (state != NULL) {
		delete m_state;
		m_state = state;
	}
}

void GPPPlayer::Update(float elapsedTime)
{
	MovableObject::Update(elapsedTime);
	PlayerState* state = m_state->Update(*this, elapsedTime);
	if (state != NULL) {
		delete m_state;
		m_state = state;
	}
}

void GPPPlayer::Render(HDC& memDC, float posInWindowX, float posInWindowY)
{
	Ellipse(memDC,
		static_cast<int>((posInWindowX - m_halfSizeX) * GPP_PIXEL_PER_METER),
		static_cast<int>((posInWindowY - m_halfSizeY) * GPP_PIXEL_PER_METER),
		static_cast<int>((posInWindowX + m_halfSizeX) * GPP_PIXEL_PER_METER),
		static_cast<int>((posInWindowY + m_halfSizeY) * GPP_PIXEL_PER_METER)
	);
}

void GPPPlayer::ShotBomb()
{
	m_rgbColor[0] = 0;
	MovableObject* bomb = GetGPPWorld()->SpawnObject<MovableObject>(m_positionX, m_positionY + 0.25f, 0.4f, 0.4f, 0, 0, 0);
	bomb->SetMass(10.0f);
	bomb->GiveForce(5000.0f, 0.0f);
	bomb->SetLifeTime(3.0f);
}
