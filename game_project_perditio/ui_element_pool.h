#pragma once
#include "ui_element.h"
#include "progress_bar.h"

class UiElementPool {
public:
	UiElementPool();
	~UiElementPool();

	void Update(PERAudio& audio, double dTime);
	void Renderer(PERRenderer& renderer);
	void RendererInWorld(PERRenderer& renderer);

	UiElement* Create(PERUiElementType type, bool inWorld = false);

private:
	void DoGarbegeCollection(double dTime);

	ProgressBar* CreateProgressBarInWorld();
	ProgressBar* CreateProgressBarOnScreen();

	ProgressBar m_progressBarsInWorld[PER_DEFAULT_UI_ELEMENT_POOL_SIZE];
	ProgressBar* m_progressBarInWorldHead;

	ProgressBar m_progressBarsOnScreen[PER_DEFAULT_UI_ELEMENT_POOL_SIZE];
	ProgressBar* m_progressBarOnScreenHead;
};
