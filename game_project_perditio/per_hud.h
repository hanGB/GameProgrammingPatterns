#pragma once
#include "event_reciver.h"
#include "per_audio.h"
#include "ui_element.h"
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

	// ���߿� �ڽ� Ŭ������ �̵�
	ProgressBar* GetBodyBar();
	ProgressBar* GetMindBar();

private:
	int PushElement(UiElement* element);

	static const int c_MAX_ELEMENTS = 128;

	std::vector<UiElement*> m_uiElements;
	int m_numElement = 0;
	int m_maxElements;

	// ���߿� �ڽ� Ŭ������ �̵�
	int m_bodyBarIndex;
	int m_mindBarIndex;
};