#pragma once
#include "stdafx.h"

class PERAudio;
class PERRenderer;

template <class T>
class UiElementPool {
public:
	UiElementPool();

	void Update(PERAudio& audio, double dTime);
	void Renderer(PERRenderer& renderer);
	void RendererInWorld(PERRenderer& renderer);

	T* CreateElementInWorld();
	T* CreateElementOnScreen();

	void DoGarbegeCollection(double dTime);

private:
	T m_elementsInWorld[PER_DEFAULT_UI_ELEMENT_POOL_SIZE];
	T* m_elementInWorldHead;

	T m_elementsOnScreen[PER_DEFAULT_UI_ELEMENT_POOL_SIZE];
	T* m_elementOnScreenHead;
};


template<class T>
UiElementPool<T>::UiElementPool()
{
	m_elementInWorldHead = &m_elementsInWorld[0];
	for (int i = 0; i < PER_DEFAULT_UI_ELEMENT_POOL_SIZE - 1; ++i) {
		m_elementsInWorld[i].SetNext(&m_elementsInWorld[i + 1]);
	}
	m_elementsInWorld[PER_DEFAULT_UI_ELEMENT_POOL_SIZE - 1].SetNext(nullptr);

	m_elementOnScreenHead = &m_elementsOnScreen[0];
	for (int i = 0; i < PER_DEFAULT_UI_ELEMENT_POOL_SIZE - 1; ++i) {
		m_elementsOnScreen[i].SetNext(&m_elementsOnScreen[i + 1]);
	}
	m_elementsOnScreen[PER_DEFAULT_UI_ELEMENT_POOL_SIZE - 1].SetNext(nullptr);
}

template<class T>
void UiElementPool<T>::Update(PERAudio& audio, double dTime)
{
	for (auto& element : m_elementsInWorld) {
		element.Update(audio, dTime);
	}
	for (auto& element : m_elementsOnScreen) {
		element.Update(audio, dTime);
	}
}

template<class T>
void UiElementPool<T>::Renderer(PERRenderer& renderer)
{
	for (auto& element : m_elementsOnScreen) {
		element.RenderOnScreen(renderer);
	}
}

template<class T>
void UiElementPool<T>::RendererInWorld(PERRenderer& renderer)
{
	for (auto& element : m_elementsInWorld) {
		element.RenderInWorld(renderer);
	}
}

template<class T>
T* UiElementPool<T>::CreateElementInWorld()
{
	if (m_elementInWorldHead == nullptr) return nullptr;

	T* newElement = m_elementInWorldHead;
	m_elementInWorldHead = dynamic_cast<T*>(newElement->GetNext());

	return newElement;
}

template<class T>
T* UiElementPool<T>::CreateElementOnScreen()
{
	if (m_elementOnScreenHead == nullptr) return nullptr;

	T* newElement = m_elementOnScreenHead;
	m_elementOnScreenHead = dynamic_cast<T*>(newElement->GetNext());

	return newElement;
}

template<class T>
void UiElementPool<T>::DoGarbegeCollection(double dTime)
{
	for (auto& element : m_elementsInWorld) {
		if (element.GetIsInUse())
		{
			if (element.IsLifeTimeIsEnd(dTime))
			{
				element.SetNext(m_elementInWorldHead);
				m_elementInWorldHead = &element;
				element.SetIsInUse(false);
			}
		}
	}
	for (auto& element : m_elementsOnScreen) {
		if (element.GetIsInUse())
		{
			if (element.IsLifeTimeIsEnd(dTime))
			{
				element.SetNext(m_elementOnScreenHead);
				m_elementOnScreenHead = &element;
				element.SetIsInUse(false);
			}
		}
	}
}