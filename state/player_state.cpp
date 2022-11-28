#include "player_state.h"

PlayerState* DuckingState::HandleInput(GPPPlayer& player, GPPInputChunk& inputs)
{
	float velocityX, velocityY;
	float mass;
	float currentAccX, currentAccY;
	player.GetVelocity(&velocityX, &velocityY);
	player.GetMass(&mass);
	player.GetCurrentAccel(&currentAccX, &currentAccY);

	if (inputs.Input_A)
	{
		if (velocityX > -MAXIUM_VELOCITY_X && !player.GetIsFalling()) {
			currentAccX -= FORCE_X / mass;
			player.SetCurrentAccel(currentAccX, currentAccY);
		}
	}
	if (inputs.Input_D)
	{
		if (velocityX < MAXIUM_VELOCITY_X && !player.GetIsFalling()) {
			currentAccX += FORCE_X / mass;
			player.SetCurrentAccel(currentAccX, currentAccY);
		}
	}
	if (!inputs.Input_S)
	{
		m_chargeTime = 0.0f;

		float halfSizeX, halfSizeY;
		player.GetHalfSize(&halfSizeX, &halfSizeY);
		halfSizeY *= 2.0f;
		player.SetHalfSize(halfSizeX, halfSizeY);
		int r, g, b;
		player.GetRGBColor(&r, &g, &b);
		player.SetRGBColor(0, g, b);
		return new StandingState();
	}
	return NULL;
}

PlayerState* DuckingState::Update(GPPPlayer& player, float elapsedTimeInSec)
{
	m_chargeTime += elapsedTimeInSec;

	int r, g, b;
	player.GetRGBColor(&r, &g, &b);
	r = static_cast<int>(MAXIMUM_RGB_BRIGHTNESS * m_chargeTime / MAX_CHARGE);
	player.SetRGBColor(r, g, b);

	if (MAX_CHARGE < m_chargeTime) {
		player.ShotBomb();
		m_chargeTime = 0.0f;
	}

	return NULL;
}

PlayerState* StandingState::HandleInput(GPPPlayer& player, GPPInputChunk& inputs)
{
	float velocityX, velocityY;
	float mass;
	float currentAccX, currentAccY;
	player.GetVelocity(&velocityX, &velocityY);
	player.GetMass(&mass);
	player.GetCurrentAccel(&currentAccX, &currentAccY);

	if (inputs.Input_A)
	{
		if (velocityX > -MAXIUM_VELOCITY_X) {
			currentAccX -= FORCE_X / mass;
			player.SetCurrentAccel(currentAccX, currentAccY);
		}
	}
	if (inputs.Input_D)
	{
		if (velocityX < MAXIUM_VELOCITY_X) {
			currentAccX += FORCE_X / mass;
			player.SetCurrentAccel(currentAccX, currentAccY);
		}
	}

	if (inputs.Input_Space)
	{
		currentAccY = FORCE_Y / mass;
		player.SetCurrentAccel(currentAccX, currentAccY);
		return new FallingState();
	}
	else if (inputs.Input_S)
	{

		float halfSizeX, halfSizeY;
		player.GetHalfSize(&halfSizeX, &halfSizeY);
		halfSizeY /= 2.0f;
		player.SetHalfSize(halfSizeX, halfSizeY);

		float positionX, positionY;
		player.GetPosition(&positionX, &positionY);
		positionY -= halfSizeY;
		player.SetPosition(positionX, positionY);

		return new DuckingState();
	}
	return NULL;
}

PlayerState* StandingState::Update(GPPPlayer& player, float elapsedTimeInSec)
{
	if (player.GetIsFalling()) {
		return new FallingState();
	}
	return NULL;
}

PlayerState* FallingState::HandleInput(GPPPlayer& player, GPPInputChunk& inputs)
{
	if (inputs.Input_S)
	{
		float velocityX, velocityY;
		player.GetVelocity(&velocityX, &velocityY);
		velocityY = -10.0f;
		player.SetVelocity(velocityX, velocityY);
		return new DivingState();
	}
	return NULL;
}

PlayerState* FallingState::Update(GPPPlayer& player, float elapsedTimeInSec)
{
	if (!player.GetIsFalling()) {
		return new StandingState();
	}
	return NULL;
}

PlayerState* DivingState::HandleInput(GPPPlayer& player, GPPInputChunk& inputs)
{
	return NULL;
}

PlayerState* DivingState::Update(GPPPlayer& player, float elapsedTimeInSec)
{
	if (!player.GetIsFalling()) {
		return new StandingState();
	}
	return NULL;
}
