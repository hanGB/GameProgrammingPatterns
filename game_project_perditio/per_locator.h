#pragma once
#include "per_audio.h"
#include "per_logger.h"

#include "null_audio.h"
#include "null_logger.h"

class PERLocator {
public:
	static void Initialize();

	static PERAudio& GetAudio();
	static PERLogger& GetLogger();

	static void Provide(PERAudio* audio, PERLogger* logger);

private:
	static PERAudio* m_audio;
	static PERLogger* m_logger;

	// null ¼­ºñ½º
	static NullAudio m_nullAudio;
	static NullLogger m_nullLogger;
};

