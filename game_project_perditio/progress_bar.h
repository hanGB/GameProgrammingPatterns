#pragma once
#include "ui_element.h"

class PERRenderer;
class PERAudio;

class ProgressBar : public UiElement {
public:
	ProgressBar(PERVec2 pos, int max, int current);
	~ProgressBar();

	virtual void Update(PERAudio& audio, double dTime);
	virtual void Render(PERRenderer& renderer);

	void SetCurrent(int current);
	void SetMax(int max);

	void SetSize(PERVec2 size);
	void SetColor(PERColor bar, PERColor progress);
	void SetBorder(bool border, int width, PERColor color);

private:
	void UpdateShowingValue(PERAudio& audio, double dTime);

	const double c_UPDATE_TIME = 0.2;

	// ��ġ, ��� ����
	PERVec2		m_position;
	PERVec2		m_size;
	PERColor	m_barColor;
	PERColor	m_progressColor;

	// �׵θ� ����
	bool		m_border;
	int			m_borderWidth;
	PERColor	m_borderColor;

	// ������ ����
	int		m_max;
	int		m_current;
	double	m_showing;
	double	m_updateSpeed;
};