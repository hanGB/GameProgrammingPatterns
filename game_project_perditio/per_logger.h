#pragma once

class PERLogger {
public:
	virtual ~PERLogger() {}

	virtual void Update() = 0;

	virtual void Info(const char* text) = 0;
	virtual void Warnning(const char* text) = 0;
	virtual void Error(const char* text) = 0;

	template<class... Args>
	void InfoWithFormat(const char* format, Args... args) {
		sprintf_s(m_buffer, m_bufferSize, format, args...);
		Info(m_buffer);
	}
	template<class... Args>
	void WarnningWithFormat(const char* format, Args... args) {
		sprintf_s(m_buffer, m_bufferSize, format, args...);
		Warnning(m_buffer);
	}
	template<class... Args>
	void ErrorWithFormat(const char* format, Args... args) {
		sprintf_s(m_buffer, m_bufferSize, format, args...);
		Error(m_buffer);
	}

	virtual void SaveLogs() = 0;
	virtual void SaveWarnnings() = 0;
	virtual void SaveErrors() = 0;
	virtual void SaveAll() = 0;

protected:
	const char* GetNowTime() {
		auto now = std::chrono::system_clock::now();
		auto time = std::chrono::system_clock::to_time_t(now);
		ctime_s(m_timeBuffer, m_timeBufferSize, &time);

		return m_timeBuffer;
	}

	char m_buffer[PER_LOG_BUFFER_SIZE];
	int m_bufferSize = PER_LOG_BUFFER_SIZE;

	char m_timeBuffer[PER_LOG_BUFFER_SIZE];
	int m_timeBufferSize = PER_LOG_BUFFER_SIZE;
};