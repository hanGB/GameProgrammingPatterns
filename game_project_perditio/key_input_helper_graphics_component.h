#pragma once
#include "graphics_component.h"

class KeyInputHelper;

class KeyInputHelperGraphicsComponent : public GraphicsComponent {
public:
	virtual void Update(PERHud& hud, PERAudio& audio, double dTime);
	virtual void Render(PERRenderer& renderer, double frameGap);
	virtual void SetData(PERComponent::GraphicsData data);
	virtual void Initialize();

	virtual void RemoveFloatingUi();

protected:
	PERVec3 m_position;
	PERVec3 m_size;

private:
	const double c_SHOWING_BODY_BAR_DISTANCE_2 = 1.0;

	void UpdateHelper(PERHud& hud);

	bool ShowHelper(PERHud& hud);
	void MatchHelperWithData();
	void HideHelper();

	bool m_isShowingHelper = false;
	KeyInputHelper* m_Helper = nullptr;
};