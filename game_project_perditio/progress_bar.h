#pragma once
#include "ui_element.h"

class PERRenderer;
class PERAudio;

class ProgressBar : public UiElement {
public:
	ProgressBar();
	~ProgressBar();
	
	void Init(PERVec2 pos, PERVec2 size, PERColor backgorund, PERColor value, int max, int current);

	virtual void MatchWithData(std::string name, int max, int current);

	virtual void Update(PERAudio& audio, double dTime);
	virtual void RenderOnScreen(PERRenderer& renderer);
	virtual void RenderInWorld(PERRenderer& renderer);

	void SetCurrent(int current);
	void SetMax(int max);

	void SetValueColor(PERColor color);

	void UpateShowingValueImmediately();

private:
	void UpdateShowingValue(PERAudio& audio, double dTime);

	const double c_UPDATE_TIME = 0.2;

	// 모양 정보
	PERColor	m_valueColor;

	// 데이터 정보
	int		m_max;
	int		m_current;
	double	m_showing;
	double	m_updateSpeed;
};