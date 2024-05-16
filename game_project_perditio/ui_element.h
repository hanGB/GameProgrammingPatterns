#pragma once

class PERController;
class PERRenderer;
class PERAudio;
class PERDatabase;

enum class PERUiElementType {
	NON,
	PROGRESS_BAR,
	NAME_TAG,
	KEY_INPUT_HELPER,
	TEXT_VIEWER,
	SELECT_BOX,
	NUM_UI_ELEMENT_TYPE
};

class UiElement {
public:
	UiElement();
	~UiElement();

	// pool관련 함수
	UiElement* GetNext() const;
	void SetNext(UiElement* next);
	bool GetIsInUse() const;
	void SetIsInUse(bool inUse);

	void Init(PERVec2 pos, PERVec2 size, PERColor backgroundColor, bool border = true, int borderWidth = 1, PERColor borderColor = PERColor(0, 0, 0));

	virtual void Update(PERController& controller, PERAudio& audio, double dTime) = 0;
	virtual void RenderOnScreen(PERRenderer& renderer, PERDatabase& database) = 0;
	virtual void RenderInWorld(PERRenderer& renderer, PERDatabase& database) = 0;

	bool IsLifeTimeEnd(double time);

	// getter
	PERVec2 GetPosition() const;
	PERVec2 GetSize() const;
	PERColor GetBackgroundColor() const;
	bool GetBorder() const;
	int GetBorderWidth() const;
	PERColor GetBorderColor() const;
	double GetLifeTime() const;

	// setter
	void SetPosition(PERVec2 position);
	void SetSize(PERVec2 size);
	void SetBackgroundColor(PERColor color);
	void SetBorder(bool border, int width, PERColor color);
	void SetLifeTime(double time);

private:
	bool m_isInUse = false;

	PERVec2	m_position;
	PERVec2	m_size;

	// 모양 정보
	PERColor	m_backgroundColor = PERColor(200, 200, 200);
	bool		m_border = true;
	int			m_borderWidth = 1;
	PERColor	m_borderColor = PERColor(0, 0, 0);

	// 라이프 타임
	double m_lifeTime = PER_MAXIMUM_LIFE_TIME;

	// 다음
	UiElement* m_next;
};