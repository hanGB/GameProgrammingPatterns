#pragma once
#include "visible_graphics_component.h"
#include "name_tag.h"

class VisibleWithInformationGraphicsComponent : public VisibleGraphicsComponent {
public:
	virtual void Update(PERObject& object, PERHud& hud, PERAudio& audio, double dTime);
	virtual void Render(PERObject& object, PERRenderer& renderer, double frameGap);
	virtual void SetData(PERComponent::GraphicsData data);
	virtual void Initialize(PERComponent::GraphicsData data);

	virtual void RemoveFloatingUi();

private:
	const double c_SHOWING_NAME_TAG_DISTANCE_2 = 10.0 * 10.0;

	void UpdateNameTag(PERObject& object, PERHud& hud);

	bool ShowNameTag(PERObject& object, PERHud& hud);
	void MatchNameTagWithData(PERObject& object);
	void HideNameTag();

	bool m_isShowingNameTag = false;
	NameTag* m_nameTag = nullptr;
};