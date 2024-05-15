#include "stdafx.h"
#include "per_audio.h"

PERAudio::PERAudio()
{
}

PERAudio::~PERAudio()
{
	delete[] m_pending;
}

void PERAudio::Update()
{
	ProcessPendingMessage();
}

void PERAudio::RequestHandleSound(PERAudioMessageId messageId, PERSoundId soundId, double volume)
{
	if (m_maxPending == m_numPending) ResizePedingArray();

	m_pending[m_numPending].messageId = messageId;
	m_pending[m_numPending].soundId = soundId;
	m_pending[m_numPending].volume = volume;
	m_numPending++;
}

void PERAudio::ProcessPendingMessage()
{
	for (int i = 0; i < m_numPending; ++i) {
		PERAudioMessage& message = m_pending[i];

		switch (message.messageId) {
		// sound
		case PERAudioMessageId::PLAY_SOUND: 
			PlaySound(message.soundId, message.volume);
			break;
		case PERAudioMessageId::STOP_SOUND:
			StopSound(message.soundId);
			break;
		case PERAudioMessageId::STOP_ALL_SOUNDS:
			StopAllSounds();
			break;
		// bgm
		case PERAudioMessageId::SET_BGM:
			SetBGM(message.soundId, message.volume);
			break;
		case PERAudioMessageId::ADD_BGM:
			AddBGM(message.soundId, message.volume);
			break;
		case PERAudioMessageId::PLAY_CURRENT_BGM:
			PlayCurrentBGM();
			break;
		case PERAudioMessageId::STOP_CURRENT_BGM:
			StopCurrentBGM();
			break;
		case PERAudioMessageId::PAUSE_CURRENT_BGM:
			PauseCurrentBGM(true);
			break;
		case PERAudioMessageId::RESUME_CURRENT_BGM:
			PauseCurrentBGM(false);
			break;
		case PERAudioMessageId::STOP_ALL_BGM:
			StopAllBGMs();
			break;
		}
	}
	m_numPending = 0;
}

void PERAudio::ResizePedingArray()
{
	// 현재 배열 크기를 2배로 증가
	m_maxPending *= 2;
	PERAudioMessage* newArray = new PERAudioMessage[m_maxPending];
	memmove(newArray, m_pending, sizeof(m_pending));

	delete[] m_pending;
	m_pending = newArray;
}
