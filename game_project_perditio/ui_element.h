#pragma once

class PERRenderer;
class PERAudio;

class UiElement {
public:
	UiElement();
	~UiElement();

	virtual void Update(PERAudio& audio, double dTime) = 0;
	virtual void Render(PERRenderer& renderer) = 0;

private:

};