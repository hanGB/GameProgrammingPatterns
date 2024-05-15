#include "stdafx.h"
#include "irr_audio.h"

IRRAudio::IRRAudio()
{
	// 엔진 생성
	m_engine = irrklang::createIrrKlangDevice();

	// 사운드 로드
	LoadSound(PERSoundId::BLADE_SWING, "./sound/swordSound.wav");
	LoadSound(PERSoundId::BULLET_SHOOTING, "./sound/triggerSound.wav");
	LoadSound(PERSoundId::SWITCH_USING, "./sound/switchEffect.wav");
	LoadSound(PERSoundId::BATTLE_BGM, "./sound/battleBGM.mp3");
}

IRRAudio::~IRRAudio()
{
	// 사운드 삭제
	for (auto& sound : m_soundMap) {
		sound.second->drop();
	}

	// 엔진 삭제
	m_engine->drop();

	PERAudio::~PERAudio();
}

void IRRAudio::Update()
{
	PERAudio::Update();
}

void IRRAudio::LoadSound(PERSoundId id, const char* fileName)
{
	if (m_soundMap.find(id) != m_soundMap.end())
	{
		PERLog::Logger().ErrorWithFormat("%s 로드 실패: ID(%d)인 사운드가 이미 있습니다", fileName, (int)id);
		return;
	}

	irrklang::ISoundSource* soundSource = m_engine->addSoundSourceFromFile(fileName);
	if (soundSource == nullptr)
	{
		PERLog::Logger().ErrorWithFormat("%s 로드 실패: 해당 파일이 없거나 호환되지 않습니다", fileName, (int)id);
		return;
	}

	m_soundMap.emplace(id, soundSource);
}

void IRRAudio::PlaySound(PERSoundId id, double volume)
{
	auto it = m_soundMap.find(id);
	if (it == m_soundMap.end())
	{
		PERLog::Logger().ErrorWithFormat("사운드 플레이 실패: ID(%d)인 사운드가 없습니다", (int)id);
		return;
	}

	it->second->setDefaultVolume((irrklang::ik_f32)volume);
	m_engine->play2D(it->second);
}

void IRRAudio::StopSound(PERSoundId id)
{
}

void IRRAudio::StopAllSounds()
{
	m_engine->stopAllSounds();
}

void IRRAudio::SetBGM(PERSoundId id, double volume)
{

	auto it = m_soundMap.find(id);
	if (it == m_soundMap.end())
	{
		PERLog::Logger().ErrorWithFormat("사운드 플레이 실패: ID(%d)인 사운드가 없습니다", (int)id);
		return;
	}

	it->second->setDefaultVolume((irrklang::ik_f32)volume);
	m_bgm = m_engine->play2D(it->second, true, true);

	if (m_bgm == nullptr)
	{
		PERLog::Logger().ErrorWithFormat("사운드 플레이 실패: 알 수 없는 이유로 BGM 플레이에 실패했습니다", (int)id);
	}
}

void IRRAudio::AddBGM(PERSoundId id, double volume)
{
}

void IRRAudio::PlayCurrentBGM()
{
	if (!m_bgm) return;

	m_bgm->setPlayPosition(0);
	m_bgm->setIsPaused(false);
}

void IRRAudio::StopCurrentBGM()
{
	if (!m_bgm) return;

	m_bgm->stop();
	m_bgm->drop();
	m_bgm = nullptr;
}

void IRRAudio::PauseCurrentBGM(bool isPause)
{
	if (!m_bgm) return;

	m_bgm->setIsPaused(isPause);
}

void IRRAudio::StopAllBGMs()
{
	if (!m_bgm) return;

	m_bgm->stop();
	m_bgm->drop();
	m_bgm = nullptr;
}
