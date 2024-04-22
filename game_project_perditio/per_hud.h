#pragma once
#include "event_reciver.h"
#include "per_audio.h"
#include "ui_element_pool.h"

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
	UiElementPool* m_uiElementPool;

};