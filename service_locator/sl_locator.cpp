#include "stdafx.h"
#include "sl_locator.h"

SlAudio* SlLocator::m_audio = nullptr;

SlLocator::~SlLocator()
{
    if (m_audio) delete m_audio;
}

SlAudio* SlLocator::GetAudio()
{
    return m_audio;
}

void SlLocator::ProvideAudio(SlAudio* audio)
{
    m_audio = audio;
}
