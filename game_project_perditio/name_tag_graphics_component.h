#pragma once
#include "graphics_component.h"
#include "name_tag.h"

class NameTagGraphicsComponent : public GraphicsComponent {
public:
	virtual void Update(PERHud& hud, PERAudio& audio, double dTime);
	virtual void Render(PERRenderer& renderer, double frameGap);
	virtual void SetData(PERComponent::GraphicsData data);
	virtual void Initialize();

	virtual void RemoveFloatingUi();

protected:
	PERVec3 m_position;

private:
	const double c_SHOWING_NAME_TAG_DISTANCE_2 = 10.0 * 10.0;

	void UpdateNameTag(PERHud& hud);

	bool ShowNameTag(PERHud& hud);
	void MatchNameTagWithData();
	void HideNameTag();

	bool m_isShowingNameTag = false;
	NameTag* m_nameTag = nullptr;

};