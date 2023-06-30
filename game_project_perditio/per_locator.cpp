#include "stdafx.h"
#include "per_locator.h"

// 클래스 내 정적 변수 선언
PERAudio* PERLocator::m_audio = nullptr;
PERLogger* PERLocator::m_logger = nullptr;
NullAudio PERLocator::m_nullAudio;
NullLogger PERLocator::m_nullLogger;

void PERLocator::Initialize()
{
    m_audio = &m_nullAudio;
    m_logger = &m_nullLogger;
}

PERAudio& PERLocator::GetAudio()
{
    return *m_audio;
}

PERLogger& PERLocator::GetLogger()
{
    return *m_logger;
}

void PERLocator::Provide(PERAudio* audio, PERLogger* logger)
{
    // audio
    if (audio == nullptr) m_audio = &m_nullAudio;
    else m_audio = audio;

    // logger
    if (logger == nullptr) m_logger = &m_nullLogger;
    else m_logger = logger;
}

void PERLocator::DeleteAllServices()
{
    if (m_audio != &m_nullAudio) delete m_audio;
    if (m_logger != &m_nullLogger) delete m_logger;
}
