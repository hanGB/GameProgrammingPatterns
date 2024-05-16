#include "stdafx.h"
#include "per_audio.h"

PERAudio::PERAudio()
{
}

PERAudio::~PERAudio()
{
	delete[] m_pending;
	delete[] m_copyPending;
}

void PERAudio::Update()
{
	ProcessPendingMessage();
}

void PERAudio::RequestMakeSound(PERAudioMessageId messageId, PERSoundId soundId, double volume, int slot)
{
	m_csProvider.Lock();

	if (m_maxPending == m_numPending) ResizePedingArray();

	m_pending[m_numPending].messageId = messageId;
	m_pending[m_numPending].soundId = soundId;
	m_pending[m_numPending].volume = volume;
	m_pending[m_numPending].slot = slot;
	m_numPending++;

	m_csProvider.Unlock();
}

void PERAudio::RequestHandleSound(PERAudioMessageId messageId, int slot)
{
	m_csProvider.Lock();

	if (m_maxPending == m_numPending) ResizePedingArray();

	m_pending[m_numPending].messageId = messageId;
	m_pending[m_numPending].slot = slot;
	m_numPending++;

	m_csProvider.Unlock();
}

void PERAudio::ProcessPendingMessage()
{
	if (m_numPending == 0) return;

	m_csProvider.Lock();

	// 메세지 페딩의 카피본을 만들어 작업 -> 사운드를 재생시키는 와중에도 메세지를 받는 게 가능해짐 
	m_numCopyPending = m_numPending;
	memcpy_s(m_copyPending, m_numPending * sizeof(PERAudioMessage), m_pending, m_numPending * sizeof(PERAudioMessage));
	m_numPending = 0;

	m_csProvider.Unlock();

	for (int i = 0; i < m_numCopyPending; ++i) {
		PERAudioMessage& message = m_copyPending[i];

		switch (message.messageId) {
			// sound
		case PERAudioMessageId::PLAY_SOUND_ONE_TIME:
			PlaySoundOneTime(message.soundId, message.volume);
			break;
		case PERAudioMessageId::STOP_ALL_SOUNDS:
			StopAllSounds();
			break;
			// bgm
		case PERAudioMessageId::SET_BGM:
			SetBGM(message.soundId, message.volume);
			break;
		case PERAudioMessageId::PLAY_BGM:
			PlayBGM();
			break;
		case PERAudioMessageId::STOP_BGM:
			StopBGM();
			break;
		case PERAudioMessageId::PAUSE_BGM:
			PauseBGM(true);
			break;
		case PERAudioMessageId::RESUME_BGM:
			PauseBGM(false);
			break;
			// ambient
		case PERAudioMessageId::SET_AMBIENT_SOUND:
			SetAmbientSound(message.soundId, message.volume, message.slot);
			break;
		case PERAudioMessageId::PLAY_AMBIENT_SOUND:
			PlayAmbientSound(message.slot);
			break;
		case PERAudioMessageId::STOP_AMBIENT_SOUND:
			StopAmbientSound(message.slot);
			break;
		case PERAudioMessageId::PAUSE_AMBIENT_SOUND:
			PauseAmbientSound(true, message.slot);
			break;
		case PERAudioMessageId::RESUME_AMBIENT_SOUND:
			PauseAmbientSound(false, message.slot);
			break;
		case PERAudioMessageId::PLAY_ALL_AMBIENT_SOUNDS:
			PlayAllAmbientSounds();
			break;
		case PERAudioMessageId::STOP_ALL_AMBIENT_SOUNDS:
			StopAllAmbientSounds();
			break;
		case PERAudioMessageId::PAUSE_ALL_AMBIENT_SOUNDS:
			PauseAllAmbientSounds(true);
			break;
		case PERAudioMessageId::RESUME_ALL_AMBIENT_SOUNDS:
			PauseAllAmbientSounds(false);
			break;
		}
	}
}

void PERAudio::ResizePedingArray()
{
	// 현재 배열 크기를 2배로 증가
	m_maxPending *= 2;
	PERAudioMessage* newArray = new PERAudioMessage[m_maxPending];
	memmove(newArray, m_pending, sizeof(m_pending));

	delete[] m_pending;
	m_pending = newArray;

	delete[] m_copyPending;
	m_copyPending = new PERAudioMessage[m_maxPending];
}
