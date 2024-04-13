#pragma once
#include "event_reciver.h"
#include "per_audio.h"
#include "ui_element_pool.h"
#include "progress_bar.h"

class PERRenderer;
class ProgressBar;

class PERHud : public EventReciver {
public:
	PERHud();
	~PERHud();

	void Update(PERAudio& audio, double dTime);
	void Renderer(PERRenderer& renderer);

	virtual void Recive(PEREvent event, PERVec3 data);

protected:
	void InitSettingForHud(UiElementPool* uiElementPool);

	int PushElementOnScreen(UiElement* element);

	std::vector<UiElement*> m_uiElementsOnScreen;

	UiElementPool* m_uiElementPool;

private:
	static const int c_MAX_ELEMENTS = 128;

	int m_maxElementOnScreen = c_MAX_ELEMENTS;
	int m_numElementOnScreen = 0;
};