#pragma once
#include "movable_object.h"

class PlayerState;

class GPPPlayer : public MovableObject {
public:
	GPPPlayer(float positionX, float positionY, float sizeX, float sizeY, int red, int green, int blue);
	~GPPPlayer();

	virtual void HandleInput(GPPInputChunk& inputs) override;

	virtual void Update(float elapsedTime) override;

	virtual void Render(HDC& memDC, float posInWindowX, float posInWindowY) override;

	void ShotBomb();

	PlayerState* m_state;
private:
	
};
