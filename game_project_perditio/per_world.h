#pragma once
#include "per_controller.h"

class PERWorld {
public:
	PERWorld();
	~PERWorld();

	void Update(PERController& controller, double dTime);

private:

};