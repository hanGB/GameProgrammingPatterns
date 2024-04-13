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
	void RendererInWorld(PERRenderer& renderer);

	virtual void Recive(PEREvent event, PERVec3 data);

	// 새로운 UI 요소 얻기
	UiElement* GetNewUiElementInPool(PERUiElementType type);
	void PushElementInWorld(UiElement* element);

protected:
	void InitSettingForHud(UiElementPool* uiElementPool);

	int PushElementOnScreen(UiElement* element);

	// 화면 위에 있는 UI 요소를 담는 벡터
	std::vector<UiElement*> m_uiElementsOnScreen;

private:
	void DoGarbegeCollection();

	void PopElementInWorld(int index);

	void DeleteAllElementsOnScreen();
	void DeleteAllElementsInWorld();

	static const int c_MAX_ELEMENTS = 128;

	UiElementPool* m_uiElementPool;

	int m_maxElementOnScreen = c_MAX_ELEMENTS;
	int m_numElementOnScreen = 0;

	// 오브젝트 위에 떠서 월드 안에 있는 UI 요소를 담는 벡터
	std::vector<UiElement*> m_uiElementsInWorld;
	int m_maxElementInWorld = c_MAX_ELEMENTS;
	int m_numElementInWorld = 0;
};