#pragma once

class PERRenderer;
class PERAudio;

class ProgressBar {
public:
	ProgressBar(PERVec2 pos, int max, int current);
	~ProgressBar();

	void Update(PERAudio* audio, double dTime);
	void Render(PERRenderer& renderer);
	void SetCurrent(int current);

	void SetSize(PERVec2 size);
	void SetColor(PERColor bar, PERColor progress);
	void SetBorder(bool border, int width, PERColor color);

private:
	void UpdateShowingValue(PERAudio* audio, double dTime);

	const double c_UPDATE_TIME = 0.2;

	// 모양 정보
	PERVec2		m_position;
	PERVec2		m_size;
	PERColor	m_barColor;
	PERColor	m_progressColor;
	// 테두리 정보
	bool		m_border;
	int			m_borderWidth;
	PERColor	m_borderColor;

	// 위치 정보
	int		m_max;
	int		m_current;
	double	m_showing;
	double	m_updateSpeed;
};