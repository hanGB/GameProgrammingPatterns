#pragma once

class PERRenderer;
class PERAudio;

class UiElement {
public:
	UiElement();
	~UiElement();

	virtual void MatchWithData(std::string name, int max, int current) = 0;

	virtual void Update(PERAudio& audio, double dTime) = 0;
	virtual void RenderInScreen(PERRenderer& renderer) = 0;
	virtual void RenderInWorld(PERRenderer& renderer) = 0;

	void SetPosition(PERVec2 position);
	void SetSize(PERVec2 size);
	void SetType(PERUiElementType type);

	PERUiElementType GetType() const;

protected:
	PERVec2	m_position;
	PERVec2	m_size;

private:
	PERUiElementType m_type;
};