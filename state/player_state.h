#pragma once
#include "stdafx.h"
#include "gpp_player.h"

class DuckingState;
class StandingState;
class FallingState;
class DivingState;

class PlayerState {
public:
	virtual ~PlayerState() {}
	virtual void HandleInput(GPPPlayer& player, GPPInputChunk& inputs) = 0;
	virtual void Update(GPPPlayer& player, float elapsedTimeInSec) = 0;

	static StandingState m_standing;
	static DuckingState m_ducking;
	static FallingState m_falling;
	static DivingState m_diving;

protected:
	const float FORCE_X = 500.0f;
	const float FORCE_Y = 10000.0f;

	const float MAXIUM_VELOCITY_X = 3.0f;
};

class DuckingState : public PlayerState {
public:
	DuckingState() : m_chargeTime(0) {}
	virtual void HandleInput(GPPPlayer& player, GPPInputChunk& inputs) override {

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
			player.m_state = reinterpret_cast<PlayerState*>(& PlayerState::m_standing);
		}
	}
	virtual void Update(GPPPlayer& player, float elapsedTimeInSec) override {
		m_chargeTime += elapsedTimeInSec;

		int r, g, b;
		player.GetRGBColor(&r, &g, &b);
		r = static_cast<int>(MAXIMUM_RGB_BRIGHTNESS * m_chargeTime / MAX_CHARGE);
		player.SetRGBColor(r, g, b);

		if (MAX_CHARGE < m_chargeTime) {
			player.ShotBomb();
			m_chargeTime = 0.0f;
		}
	}

private:
	const float MAX_CHARGE = 1.0f;

	float m_chargeTime;
};

class StandingState : public PlayerState {
public:
	StandingState() {}
	virtual void HandleInput(GPPPlayer& player, GPPInputChunk& inputs) override {

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
			player.m_state = reinterpret_cast<PlayerState*>(&PlayerState::m_falling);
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

			player.m_state = reinterpret_cast<PlayerState*>(&PlayerState::m_ducking);
		}
	}
	virtual void Update(GPPPlayer& player, float elapsedTimeInSec) override {
		if (player.GetIsFalling()) {
			player.m_state = reinterpret_cast<PlayerState*>(&PlayerState::m_falling);
		}
	}
};

class FallingState : public PlayerState {
public:
	FallingState() {}
	virtual void HandleInput(GPPPlayer& player, GPPInputChunk& inputs) override {
		if (inputs.Input_S)
		{
			float velocityX, velocityY;
			player.GetVelocity(&velocityX, &velocityY);
			velocityY = -10.0f;
			player.SetVelocity(velocityX, velocityY);
			player.m_state = reinterpret_cast<PlayerState*>(&PlayerState::m_diving);
		}
	}
	virtual void Update(GPPPlayer& player, float elapsedTimeInSec) override {
		if (!player.GetIsFalling()) {
			player.m_state = reinterpret_cast<PlayerState*>(&PlayerState::m_standing);
		}
	}
};

class DivingState : public PlayerState {
public:
	DivingState() {}
	virtual void HandleInput(GPPPlayer& player, GPPInputChunk& inputs) override {

	}
	virtual void Update(GPPPlayer& player, float elapsedTimeInSec) override {
		if (!player.GetIsFalling()) {
			player.m_state = reinterpret_cast<PlayerState*>(&PlayerState::m_standing);
		}
	}
};

StandingState PlayerState::m_standing = StandingState();
DuckingState PlayerState::m_ducking = DuckingState();
FallingState PlayerState::m_falling = FallingState();
DivingState PlayerState::m_diving = DivingState();
