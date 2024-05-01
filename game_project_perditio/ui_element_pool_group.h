#pragma once
#include "ui_element_pool.h"
#include "ui_element.h"
#include "progress_bar.h"
#include "name_tag.h"
#include "key_input_helper.h"

class PERDatabase;

class UiElementPoolGroup {
public:
	UiElementPoolGroup();
	~UiElementPoolGroup();

	void Update(PERController& controller, PERAudio& audio, double dTime);
	void Renderer(PERRenderer& renderer, PERDatabase& database);
	void RendererInWorld(PERRenderer& renderer, PERDatabase& database);

	UiElement* Create(PERUiElementType type, bool inWorld = false);

private:
	void DoGarbegeCollection(double dTime);

	UiElementPool<ProgressBar> m_progressBarPool;
	UiElementPool<NameTag> m_nameTagPool;
	UiElementPool<KeyInputHelper> m_keyInputHelperPool;
};
