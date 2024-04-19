#include "stdafx.h"
#include "ui_element_pool.h"

UiElementPool::UiElementPool()
{
	m_progressBarInWorldHead = &m_progressBarsInWorld[0];
	for (int i = 0; i < PER_DEFAULT_UI_ELEMENT_POOL_SIZE - 1; ++i) {
		m_progressBarsInWorld[i].SetNext(&m_progressBarsInWorld[i + 1]);
	}
	m_progressBarsInWorld[PER_DEFAULT_UI_ELEMENT_POOL_SIZE - 1].SetNext(nullptr);

	m_progressBarOnScreenHead = &m_progressBarsOnScreen[0];
	for (int i = 0; i < PER_DEFAULT_UI_ELEMENT_POOL_SIZE - 1; ++i) {
		m_progressBarsOnScreen[i].SetNext(&m_progressBarsOnScreen[i + 1]);
	}
	m_progressBarsOnScreen[PER_DEFAULT_UI_ELEMENT_POOL_SIZE - 1].SetNext(nullptr);
}

UiElementPool::~UiElementPool()
{
}

ProgressBar* UiElementPool::CreateProgressBarInWorld()
{
	if (m_progressBarInWorldHead == nullptr) return nullptr;

	ProgressBar* newProgressBar = m_progressBarInWorldHead;
	m_progressBarInWorldHead = dynamic_cast<ProgressBar*>(newProgressBar->GetNext());

	return newProgressBar;
}

ProgressBar* UiElementPool::CreateProgressBarOnScreen()
{
	if(m_progressBarOnScreenHead == nullptr) return nullptr;

	ProgressBar* newProgressBar = m_progressBarOnScreenHead;
	m_progressBarOnScreenHead = dynamic_cast<ProgressBar*>(newProgressBar->GetNext());

	return newProgressBar;
}

void UiElementPool::Update(PERAudio& audio, double dTime)
{
	DoGarbegeCollection(dTime);

	for (auto& progressBar : m_progressBarsInWorld) {
		progressBar.Update(audio, dTime);
	}
	for (auto& progressBar : m_progressBarsOnScreen) {
		progressBar.Update(audio, dTime);
	}
}

void UiElementPool::Renderer(PERRenderer& renderer)
{
	for (auto& progressBar : m_progressBarsOnScreen) {
		progressBar.RenderOnScreen(renderer);
	}
}

void UiElementPool::RendererInWorld(PERRenderer& renderer)
{
	for (auto& progressBar : m_progressBarsInWorld) {
		progressBar.RenderInWorld(renderer);
	}
}

UiElement* UiElementPool::Create(PERUiElementType type, bool inWorld)
{
	if (inWorld)
	{
		switch (type) {
		case PERUiElementType::PROGRESS_BAR: return CreateProgressBarInWorld();
		}
	}
	else
	{
		switch (type) {
		case PERUiElementType::PROGRESS_BAR: return CreateProgressBarOnScreen();
		}
	}

	return nullptr;
}

void UiElementPool::DoGarbegeCollection(double dTime)
{
	for (auto& progressBar : m_progressBarsOnScreen) {
		if (progressBar.GetIsInUse()) 
		{
			if (progressBar.IsLifeTimeIsEnd(dTime)) 
			{
				progressBar.SetNext(m_progressBarOnScreenHead);
				m_progressBarOnScreenHead = &progressBar;
				progressBar.SetIsInUse(false);
			}
		}
	}
	for (auto& progressBar : m_progressBarsInWorld) {
		if (progressBar.GetIsInUse())
		{
			if (progressBar.IsLifeTimeIsEnd(dTime))
			{
				progressBar.SetNext(m_progressBarInWorldHead);
				m_progressBarOnScreenHead = &progressBar;
				progressBar.SetIsInUse(false);
			}
		}
	}
}
