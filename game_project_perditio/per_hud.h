#pragma once
#include "event_reciver.h"
#include "per_audio.h"
#include "ui_element_pool_group.h"

class PERController;
class PERRenderer;
class PERDatabase;

class PERHud : public EventReciver {
public:
	PERHud();
	~PERHud();

	void Update(PERController& controller, PERAudio& audio, double dTime);
	void Renderer(PERRenderer& renderer, PERDatabase& database);
	void RendererInWorld(PERRenderer& renderer, PERDatabase& database);

	virtual void Recive(PEREvent event, PERVec3 data);

	UiElement* GetNewUiElementInWorld(PERUiElementType type);

protected:
	UiElementPoolGroup* m_uiElementPool;

};