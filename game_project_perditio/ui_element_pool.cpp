#include "stdafx.h"
#include "ui_element_pool.h"

#include "progress_bar.h"

UiElementPool::UiElementPool()
{
	PERLog::Logger().Info("UI 夸家 钱 积己");

	FillElementPools();
}

UiElementPool::~UiElementPool()
{
	PERLog::Logger().Info("UI 夸家 钱 昏力");

	ClearElemenPools();
}

UiElement* UiElementPool::PopElement(PERUiElementType type)
{
    auto& pool = m_elementPools.find(type)->second;

    if (pool.empty()) RefillElementPool(pool, type);

    UiElement* element = pool.front();
    pool.pop();

	return element;
}

void UiElementPool::PushElement(PERUiElementType type, UiElement* element)
{
    auto& pool = m_elementPools.find(type)->second;
    pool.push(element);
}

void UiElementPool::FillElementPools()
{
    // progress bar
    std::queue<UiElement*> progressBarPool;
    RefillElementPool(progressBarPool, PERUiElementType::PROGRESS_BAR);
    m_elementPools.insert(std::pair<PERUiElementType, std::queue<UiElement*>>(PERUiElementType::PROGRESS_BAR, progressBarPool));
}

void UiElementPool::RefillElementPool(std::queue<UiElement*>& pool, PERUiElementType type)
{
    switch (type) 
    {
    case PERUiElementType::PROGRESS_BAR: 
    {
        RefillElementPool<ProgressBar>(pool);
        break;
    }
    }
}

void UiElementPool::ClearElemenPools()
{
    for (int type = (int)PERUiElementType::NON + 1; type < (int)PERUiElementType::NUM_UI_ELEMENT_TYPE; ++type) {

        auto it = m_elementPools.find((PERUiElementType)type);
        if (it == m_elementPools.end()) continue;

        std::queue<UiElement*> pool = it->second;
        while (!pool.empty()) {
            UiElement* element = pool.front();
            pool.pop();
            delete element;
        }
    }
}
