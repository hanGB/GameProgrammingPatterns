#pragma once
#include "gpp_game_object.h"

class Ground : public GPPGameObject {
public:
	Ground(float positionX, float positionY, float sizeX, float sizeY) {

		SetPosition(positionX, positionY);
		SetHalfSize(sizeX / 2.0f, sizeY / 2.0f);
		SetRGBColor(127, 63, 0);
	}
	~Ground() {}

	virtual void Update(float elapsedTime) override {

	}
	virtual void Render(HDC& memDC, float posInWindowX, float posInWindowY) override {

		Rectangle(memDC,
			static_cast<int>((posInWindowX - m_halfSizeX) * GPP_PIXEL_PER_METER),
			static_cast<int>((posInWindowY - m_halfSizeY) * GPP_PIXEL_PER_METER),
			static_cast<int>((posInWindowX + m_halfSizeX) * GPP_PIXEL_PER_METER),
			static_cast<int>((posInWindowY + m_halfSizeY) * GPP_PIXEL_PER_METER)
		);
	}

private:
};