#pragma once

class PERObject;
class PERRenderer;

class PERGraphicsComponent {
public:
	virtual ~PERGraphicsComponent() {}

	virtual void Update(PERObject& object, double dTime) = 0;
	virtual void Render(PERObject& object, PERRenderer& renderer) = 0;
};