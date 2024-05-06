#pragma once
#include "visible_with_information_graphics_componenet.h"

class KeyInputHelper;

class ButtonGraphicsComponent : public VisibleWithInformationGraphicsComponent {
public:
	virtual void Update(PERHud& hud, PERAudio& audio, double dTime);
	virtual void Render(PERRenderer& renderer, double frameGap);
	virtual void SetData(PERComponent::GraphicsData data);
	virtual void Initialize(PERComponent::GraphicsData data);

	virtual void RemoveFloatingUi();

private:
	const double c_SHOWING_BODY_BAR_DISTANCE_2 = 1.0;

	void UpdateHelper(PERHud& hud);

	bool ShowHelper(PERHud& hud);
	void MatchHelperWithData();
	void HideHelper();

	bool m_isShowingHelper = false;
	KeyInputHelper* m_Helper = nullptr;
};