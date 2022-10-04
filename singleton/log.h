#pragma once
#include "stdafx.h"

class Log {
public:
	Log() { }

	void WriteLog(const char* log) {
		std::cout << m_number  << ": " << log << std::endl;
		m_number++;
	}

private:
	int m_number = 0;
};