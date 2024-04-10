#pragma once

class PERRenderer;
class PERAudio;

class UiElement {
public:
	UiElement();
	~UiElement();

	virtual void MatchWithData(std::string name, int max) = 0;
	virtual void InitializeData() = 0;

	virtual void Update(PERAudio& audio, double dTime) = 0;
	virtual void RenderInScreen(PERRenderer& renderer) = 0;
	virtual void RenderInWorld(PERRenderer& renderer) = 0;

	void SetPosition(PERVec2 position);
	void SetSize(PERVec2 size);

protected:
	PERVec2	m_position;
	PERVec2	m_size;
};