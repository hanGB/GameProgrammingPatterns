#pragma once
#include "event_reciver.h"

class PERHud : public EventReciver {
public:
	PERHud();
	~PERHud();

	void Update(double dTime);

	virtual void Recive(PEREvent event, PERVec3 data);
};