#pragma once
#include "per_logger.h"

class NullLogger : public PERLogger {
public:
	virtual void ShowLog(const char* text) {}
	virtual void ShowWarnning(const char* text) {}
	virtual void ShowError(const char* text) {}

	virtual void SaveLogs() {}
	virtual void SaveWarnnings() {}
	virtual void SaveErrors() {}
	virtual void SaveAll() {}
};