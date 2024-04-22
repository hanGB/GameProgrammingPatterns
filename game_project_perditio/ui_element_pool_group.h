#pragma once
#include "ui_element_pool.h"
#include "ui_element.h"
#include "progress_bar.h"
#include "name_tag.h"

class UiElementPoolGroup {
public:
	UiElementPoolGroup();
	~UiElementPoolGroup();

	void Update(PERAudio& audio, double dTime);
	void Renderer(PERRenderer& renderer);
	void RendererInWorld(PERRenderer& renderer);

	UiElement* Create(PERUiElementType type, bool inWorld = false);

private:
	void DoGarbegeCollection(double dTime);

	UiElementPool<ProgressBar> m_progressBarPool;
	UiElementPool<NameTag> m_nameTagPool;
};
