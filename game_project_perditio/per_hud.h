#pragma once
#include "event_reciver.h"
#include "per_audio.h"
#include "ui_element_pool_group.h"

class PERRenderer;

class PERHud : public EventReciver {
public:
	PERHud();
	~PERHud();

	void Update(PERAudio& audio, double dTime);
	void Renderer(PERRenderer& renderer);
	void RendererInWorld(PERRenderer& renderer);

	virtual void Recive(PEREvent event, PERVec3 data);

	UiElement* GetNewUiElementInWorld(PERUiElementType type);

protected:
	UiElementPoolGroup* m_uiElementPool;

};