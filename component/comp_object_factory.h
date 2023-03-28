#pragma once
#include "comp_object.h"

class CompObjectFactory {
public:
	CompObject* CreateBjorn();
	CompObject* CreateDemoBjorn();

private:
	void SetDefaultValuesForBjorn(CompObject& object);
};