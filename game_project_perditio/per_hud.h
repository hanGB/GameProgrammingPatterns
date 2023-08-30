#pragma once
#include "event_reciver.h"
#include "progress_bar.h"
#include "per_audio.h"

class PERRenderer;

class PERHud : public EventReciver {
public:
	PERHud();
	~PERHud();

	void Update(PERAudio& audio, double dTime);
	void Renderer(PERRenderer& renderer);

	virtual void Recive(PEREvent event, PERVec3 data);

	ProgressBar* GetBodyBar();
	ProgressBar* GetMindBar();

private:
	ProgressBar* m_bodyBar;
	ProgressBar* m_mindBar;
};