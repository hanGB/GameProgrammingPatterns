#include "stdafx.h"
#include "sl_locator.h"

SlNullAudio SlLocator::m_nullAudio;
SlAudio* SlLocator::m_audio = nullptr;

void SlLocator::Initialize()
{
    m_audio = &m_nullAudio;
}

SlAudio* SlLocator::GetAudio()
{
    return m_audio;
}

void SlLocator::ProvideAudio(SlAudio* audio)
{
    if (audio == nullptr) m_audio = &m_nullAudio;
    else m_audio = audio;
}
