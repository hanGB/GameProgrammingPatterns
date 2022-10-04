#pragma once
#include "stdafx.h"

class OneInstanceClass {
public:
	OneInstanceClass() {
		assert(!m_instantiated);
		m_instantiated = true;
	}
	~OneInstanceClass() {
		m_instantiated = false;
	}

private:
	static bool m_instantiated;
};

bool OneInstanceClass::m_instantiated = false;
