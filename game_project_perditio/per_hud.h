#pragma once
#include "event_reciver.h"

class PERRenderer;

struct PERUIValue {
	int max;
	int current;
	double showing;
	double updateSpeed;
};

class PERHud : public EventReciver {
public:
	PERHud();
	~PERHud();

	void Update(double dTime);
	void Renderer(PERRenderer& renderer);

	virtual void Recive(PEREvent event, PERVec3 data);

private:
	void UpdateShowingValue(PERUIValue& value, double dTime);

	const double c_UPDATE_TIME = 0.5;

	PERUIValue m_hp;
	PERUIValue m_mp;
};