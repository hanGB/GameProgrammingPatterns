#include "stdafx.h"
#include "progress_bar.h"
#include "per_renderer.h"

ProgressBar::ProgressBar(PERVec2 pos, int max, int current)
	: m_position(pos), m_max(max), m_current(m_current)
{
	m_size = PERVec2(0.4, 0.1);
	m_barColor = PERColor(255, 255, 255);
	m_progressColor = PERColor(127, 127, 127);

	m_border = true;
	m_borderWidth = 1;
	m_borderColor = PERColor(0, 0, 0);

	m_showing = current;
	m_updateSpeed = 0.0;
}

ProgressBar::~ProgressBar()
{
}

void ProgressBar::Update(double dTime)
{
	if (m_showing != m_current) UpdateShowingValue(dTime);
}

void ProgressBar::Render(PERRenderer& renderer)
{
	// 바
	renderer.RenderShapeInScreenCoordinate(PERShapeType::SHAPE_TYPE_RECTANGLE_WITH_LEFT_TOP_ANCHOR,
		m_position, m_size, 
		m_barColor, m_border, m_borderWidth, m_borderColor);
	// 진행 상태
	renderer.RenderShapeInScreenCoordinate(PERShapeType::SHAPE_TYPE_RECTANGLE_WITH_LEFT_TOP_ANCHOR,
		m_position, PERVec2(m_size.x * m_showing / (double)m_max, m_size.y),
		m_progressColor, false);
}

void ProgressBar::SetCurrent(int current)
{
	m_current = current;
	m_updateSpeed = m_current - m_showing;
}

void ProgressBar::SetSize(PERVec2 size)
{
	m_size = size;
}

void ProgressBar::SetColor(PERColor bar, PERColor progress)
{
	m_barColor = bar;
	m_progressColor = progress;
}

void ProgressBar::SetBorder(bool border, int width, PERColor color)
{
	m_border = border;
	m_borderWidth = width;
	m_borderColor = color;
}

void ProgressBar::UpdateShowingValue(double dTime)
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
