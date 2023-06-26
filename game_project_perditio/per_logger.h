#pragma once

class PERLogger {
public:
	virtual ~PERLogger() {}
	virtual void ShowLog(const char* text) = 0;
	virtual void ShowWarnning(const char* text) = 0;
	virtual void ShowError(const char* text) = 0;

	virtual void SaveLogs() = 0;
	virtual void SaveWarnnings() = 0;
	virtual void SaveErrors() = 0;
	virtual void SaveAll() = 0;
};