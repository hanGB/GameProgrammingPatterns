#pragma once
#include "ui_element.h"

class PERRenderer;
class PERAudio;

class ProgressBar : public UiElement {
public:
	ProgressBar(PERVec2 pos, int max, int current);
	~ProgressBar();
	
	virtual void MatchWithData(std::string name, int max);
	virtual void InitializeData();

	virtual void Update(PERAudio& audio, double dTime);
	virtual void RenderInScreen(PERRenderer& renderer);
	virtual void RenderInWorld(PERRenderer& renderer);

	void SetCurrent(int current);
	void SetMax(int max);

	void SetColor(PERColor bar, PERColor progress);
	void SetBorder(bool border, int width, PERColor color);

	void UpateShowingValueImmediately();

private:
	void UpdateShowingValue(PERAudio& audio, double dTime);

	const double c_UPDATE_TIME = 0.2;

	// 모양 정보
	PERColor	m_barColor;
	PERColor	m_progressColor;

	// 테두리 정보
	bool		m_border;
	int			m_borderWidth;
	PERColor	m_borderColor;

	// 데이터 정보
	int		m_max;
	int		m_current;
	double	m_showing;
	double	m_updateSpeed;
};