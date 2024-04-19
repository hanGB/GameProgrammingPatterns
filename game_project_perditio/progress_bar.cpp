#include "stdafx.h"
#include "progress_bar.h"
#include "per_renderer.h"

ProgressBar::ProgressBar()
{
	
}

ProgressBar::~ProgressBar()
{
}

void ProgressBar::Init(PERVec2 pos, PERVec2 size, PERColor backgorund, PERColor value, int max, int current)
{
	UiElement::Init(pos, size, backgorund);

	m_valueColor = value;
	m_max = max;
	m_current = current;

	m_showing = (double)current;
	m_updateSpeed = 0.0;
}

void ProgressBar::MatchWithData(std::string name, int max, int current)
{
	SetMax(max);
	SetCurrent(current);
	UpateShowingValueImmediately();
}

void ProgressBar::Update(PERAudio& audio, double dTime)
{
	if (!GetIsInUse()) return;

	if (m_showing != m_current) UpdateShowingValue(audio, dTime);
}

void ProgressBar::RenderOnScreen(PERRenderer& renderer)
{
	if (!GetIsInUse()) return;

	PERVec2 pos = GetPosition();
	PERVec2 size = GetSize();
	// 바
	renderer.RenderShapeInScreenCoordinate(PERShapeType::RECTANGLE_WITH_LEFT_TOP_ANCHOR,
		pos, size, GetBackgroundColor(), GetBorder(), GetBorderWidth(), GetBorderColor());
	// 진행 상태
	renderer.RenderShapeInScreenCoordinate(PERShapeType::RECTANGLE_WITH_LEFT_TOP_ANCHOR,
		pos, PERVec2(size.x * m_showing / (double)m_max, size.y), m_valueColor, false);
}

void ProgressBar::RenderInWorld(PERRenderer& renderer)
{
	if (!GetIsInUse()) return;

	PERVec2 pos = GetPosition();
	PERVec2 size = GetSize();
	// 바
	renderer.RenderShapeInWorldCoordinate(PERShapeType::RECTANGLE_WITH_LEFT_TOP_ANCHOR,
		PERVec3(pos.x - size.x / 2.0, pos.y, 0.0), PERVec3(size.x, size.y, 0.0),
		GetBackgroundColor(), GetBorder(), GetBorderWidth(), GetBorderColor());
	// 진행 상태
	renderer.RenderShapeInWorldCoordinate(PERShapeType::RECTANGLE_WITH_LEFT_TOP_ANCHOR,
		PERVec3(pos.x - size.x / 2.0, pos.y, 0.0), PERVec3(size.x * m_showing / (double)m_max, size.y, 0.0),
		m_valueColor, false);
}

void ProgressBar::SetCurrent(int current)
{
	current = std::clamp(current, 0, m_max);
	m_current = current;
	m_updateSpeed = m_current - m_showing;
}

void ProgressBar::SetMax(int max)
{
	m_max = max;
}

void ProgressBar::SetValueColor(PERColor color)
{
	m_valueColor = color;
}

void ProgressBar::UpateShowingValueImmediately()
{
	m_showing = m_current;
	m_updateSpeed = 0.0;
}

void ProgressBar::UpdateShowingValue(PERAudio& audio, double dTime)
{
	// 현재 값이랑 보이는 값이 같아지도록 증감
	if (m_updateSpeed > 0.0) {
		if (m_current < m_showing) {
			m_showing = m_current;
		}
		else {
			m_showing += m_updateSpeed * 1.0 / c_UPDATE_TIME * dTime;
		}
	}
	else if (m_updateSpeed < 0.0) {
		if (m_current > m_showing) {
			m_showing = m_current;
		}
		else {
			m_showing += m_updateSpeed * 1.0 / c_UPDATE_TIME * dTime;
		}
	}
}
