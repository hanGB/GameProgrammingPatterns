#pragma once
#include "stdafx.h"
#include "gpp_player.h"

class PlayerState {
public:
	virtual ~PlayerState() {}
	virtual void Enter(GPPPlayer& player) = 0;
	virtual PlayerState* HandleInput(GPPPlayer& player, GPPInputChunk& inputs) = 0;
	virtual PlayerState* Update(GPPPlayer& player, float elapsedTimeInSec) = 0;

protected:
	const float FORCE_X = 500.0f;
	const float FORCE_Y = 10000.0f;

	const float MAXIUM_VELOCITY_X = 3.0f;
};

class DuckingState : public PlayerState {
public:
	DuckingState() : m_chargeTime(0) {}
	virtual void Enter(GPPPlayer& player) override;
	virtual PlayerState* HandleInput(GPPPlayer& player, GPPInputChunk& inputs) override;
	virtual PlayerState* Update(GPPPlayer& player, float elapsedTimeInSec) override;

private:
	const float MAX_CHARGE = 1.0f;

	float m_chargeTime;
};

class StandingState : public PlayerState {
public:
	StandingState() {}
	virtual void Enter(GPPPlayer& player) override;
	virtual PlayerState* HandleInput(GPPPlayer& player, GPPInputChunk& inputs) override;
	virtual PlayerState* Update(GPPPlayer& player, float elapsedTimeInSec) override;
};

class FallingState : public PlayerState {
public:
	FallingState() {}
	virtual void Enter(GPPPlayer& player) override;
	virtual PlayerState* HandleInput(GPPPlayer& player, GPPInputChunk& inputs) override;
	virtual PlayerState* Update(GPPPlayer& player, float elapsedTimeInSec) override;
};

class DivingState : public PlayerState {
public:
	DivingState() {}
	virtual void Enter(GPPPlayer& player) override;
	virtual PlayerState* HandleInput(GPPPlayer& player, GPPInputChunk& inputs) override;
	virtual PlayerState* Update(GPPPlayer& player, float elapsedTimeInSec) override;
};
