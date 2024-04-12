#pragma once
#include "ui_element.h"
#include <unordered_map>
#include <queue>

class UiElementPool {
public:
	UiElementPool();
	~UiElementPool();

	UiElement* PopElement(PERUiElementType type);
	void PushElement(PERUiElementType type, UiElement* element);

private:
	void FillElementPools();
	void RefillElementPool(std::queue<UiElement*>& pool, PERUiElementType type);
	template<class T>
	void RefillElementPool(std::queue<UiElement*>& pool)
	{
		for (size_t i = 0; i < PER_DEFAULT_UI_ELEMENT_POOL_SIZE; i++) {
			pool.push(new T());
		}
	}
	void ClearElemenPools();

	// 오브젝트 풀
	std::unordered_map<PERUiElementType, std::queue<UiElement*>> m_elementPools;
};