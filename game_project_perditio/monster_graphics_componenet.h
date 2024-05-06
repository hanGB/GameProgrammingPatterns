#pragma once
#include "visible_with_information_graphics_componenet.h"
#include "progress_bar.h"

class MonsterGraphicsComponent : public VisibleWithInformationGraphicsComponent {
public:
	virtual void Update(PERHud& hud, PERAudio& audio, double dTime);
	virtual void Render(PERRenderer& renderer, double frameGap);
	virtual void SetData(PERComponent::GraphicsData data);
	virtual void Initialize(PERComponent::GraphicsData data);

	virtual void RemoveFloatingUi();

private:
	const double c_SHOWING_BODY_BAR_DISTANCE_2 = 5.0 * 5.0;

	void UpdateBodyBar(PERHud& hud);

	bool ShowBodyBar( PERHud& hud);
	void MatchBodyBarWithData();
	void HideBodyBar();

	bool m_isShowingBodyBar = false;
	ProgressBar* m_bodyBar = nullptr;
};