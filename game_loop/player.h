#pragma once
#include "object.h"

class Player : public Object {
public:
	Player();
	~Player();

	virtual void ProcessInputs(KeyInputs& inputs);
	virtual void Update();
	virtual void Render();

	void RenderLevel();

private:
	void RenderLevelUpPopup();

	int m_level = 1;
	int m_exp = 0;
	int m_showLevelUpPopupTimer = 0;
};
