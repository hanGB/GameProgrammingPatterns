#include "stdafx.h"
#include "irr_audio.h"

IRRAudio::IRRAudio()
{
	// 엔진 생성
	m_engine = irrklang::createIrrKlangDevice();
	if (m_engine == nullptr)
	{
		PERLog::Logger().Error("사운드 엔진 생성 실패");
		return;
	}

	// 사운드 로드
	LoadSound(PERSoundId::BLADE_SWING, "./sound/swordSound.wav");
	LoadSound(PERSoundId::BULLET_SHOOTING, "./sound/triggerSound.wav");
	LoadSound(PERSoundId::SWITCH_USING, "./sound/switchEffect.wav");
	LoadSound(PERSoundId::MONSTER_DYING, "./sound/igSound.wav");
	LoadSound(PERSoundId::PLAYER_LEVEL_UP, "./sound/fatSound.wav");
	LoadSound(PERSoundId::BATTLE_BGM, "./sound/battleBGM.mp3");
	LoadSound(PERSoundId::RAINNY_BGM, "./sound/rainWithLightningSound.wav");
}

IRRAudio::~IRRAudio()
{
	StopBGM();
	StopAllAmbientSounds();

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

void IRRAudio::PlaySoundOneTime(PERSoundId id, double volume)
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
	m_backgroundMusic = m_engine->play2D(it->second, true, true);

	if (m_backgroundMusic == nullptr)
	{
		PERLog::Logger().ErrorWithFormat("사운드 플레이 실패: 알 수 없는 이유로 BGM 플레이에 실패했습니다", (int)id);
	}
}

void IRRAudio::PlayBGM()
{
	if (!m_backgroundMusic) return;

	m_backgroundMusic->setPlayPosition(0);
	m_backgroundMusic->setIsPaused(false);
}

void IRRAudio::StopBGM()
{
	if (!m_backgroundMusic) return;
	
	m_backgroundMusic->stop();
	m_backgroundMusic->drop();
	m_backgroundMusic = nullptr;
}

void IRRAudio::PauseBGM(bool isPause)
{
	if (!m_backgroundMusic) return;

	m_backgroundMusic->setIsPaused(isPause);;
}

void IRRAudio::SetAmbientSound(PERSoundId id, double volume, int slot)
{
	// 해당 슬롯에 이미 재생 중인 환경음이 있을 경우를 대비해 종료 시도
	StopAmbientSound(slot);

	auto it = m_soundMap.find(id);
	if (it == m_soundMap.end())
	{
		PERLog::Logger().ErrorWithFormat("사운드 플레이 실패: ID(%d)인 사운드가 없습니다", (int)id);
		return;
	}

	it->second->setDefaultVolume((irrklang::ik_f32)volume);
	m_ambientSounds[slot] = m_engine->play2D(it->second, true, true);

	if (m_ambientSounds[slot] == nullptr)
	{
		PERLog::Logger().ErrorWithFormat("사운드 플레이 실패: 알 수 없는 이유로 BGM 플레이에 실패했습니다", (int)id);
	}
}

void IRRAudio::PlayAmbientSound(int slot)
{
	if (!m_ambientSounds[slot]) return;

	m_ambientSounds[slot]->setPlayPosition(0);
	m_ambientSounds[slot]->setIsPaused(false);
}

void IRRAudio::StopAmbientSound(int slot)
{
	if (!m_ambientSounds[slot]) return;

	m_ambientSounds[slot]->stop();
	m_ambientSounds[slot]->drop();
	m_ambientSounds[slot] = nullptr;
}

void IRRAudio::PauseAmbientSound(bool isPause, int slot)
{
	if (!m_ambientSounds[slot]) return;

	m_ambientSounds[slot]->setIsPaused(isPause);;
}

void IRRAudio::PlayAllAmbientSounds()
{
	for (int i = 0; i < PER_MAXIMUM_AMBIEMNT_SOUND_SLOT; ++i) {
		PlayAmbientSound(i);
	}
}

void IRRAudio::StopAllAmbientSounds()
{
	for (int i = 0; i < PER_MAXIMUM_AMBIEMNT_SOUND_SLOT; ++i) {
		StopAmbientSound(i);
	}
}

void IRRAudio::PauseAllAmbientSounds(bool isPause)
{
	for (int i = 0; i < PER_MAXIMUM_AMBIEMNT_SOUND_SLOT; ++i) {
		PauseAmbientSound(isPause, i);
	}
}
