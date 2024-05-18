#pragma once
#include "graphics_component.h"

class TextGraphicsComponent : public GraphicsComponent {
public:
	virtual void Update(PERHud& hud, PERAudio& audio, double dTime);
	virtual void Render(PERRenderer& renderer, double frameGap);
	virtual void SetData(PERComponent::GraphicsData data);
	virtual void Initialize();

	// 오브젝트 사망시 제거용
	virtual void RemoveFloatingUi();

	void SetText(const wchar_t* text, PERColor color);

protected:
	PERVec3 m_position;
	double m_size;

private:
	std::wstring m_text;
	PERColor m_color;
};