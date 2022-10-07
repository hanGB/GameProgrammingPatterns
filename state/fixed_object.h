#pragma once
#include "gpp_game_object.h"

class Ground : public GPPGameObject {
public:
	Ground(float positionX, float positionY, float sizeX, float sizeY, int red, int green, int blue) {

		SetPosition(positionX, positionY);
		SetHalfSize(sizeX / 2.0f, sizeY / 2.0f);
		SetRGBColor(red, green, blue);
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