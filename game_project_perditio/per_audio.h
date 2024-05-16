#pragma once

enum class PERSoundId {
	// short sound
	BLADE_SWING,
	BULLET_SHOOTING,
	SWITCH_USING,
	MONSTER_DYING,
	PLAYER_LEVEL_UP,

	// BGM
	BATTLE_BGM,
	RAINNY_BGM,

	// num
	NUM_SOUND_ID
};

enum class PERAudioMessageId {
	// 효과음
	PLAY_SOUND,
	STOP_ALL_SOUNDS,
	// 배경음악
	SET_BGM,
	PLAY_BGM,
	STOP_BGM,
	PAUSE_BGM,
	RESUME_BGM,
	// 환경음
	SET_AMBIENT_SOUND,
	PLAY_AMBIENT_SOUND,
	STOP_AMBIENT_SOUND,
	PAUSE_AMBIENT_SOUND,
	RESUME_AMBIENT_SOUND,
	PLAY_ALL_AMBIENT_SOUNDS,
	STOP_ALL_AMBIENT_SOUNDS,
	PAUSE_ALL_AMBIENT_SOUNDS,
	RESUME_ALL_AMBIENT_SOUNDS,

	NUM_WROLD_MESSAGE_ID
};

struct PERAudioMessage {
	PERAudioMessageId messageId;
	PERSoundId soundId;
	double volume;
	int slot;
};

class PERAudio {
public:
	PERAudio();
	~PERAudio();

	virtual void Update();

	virtual void LoadSound(PERSoundId id, const char* fileName) = 0;
	void RequestMakeSound(PERAudioMessageId messageId, PERSoundId soundId, double volume = 1.0, int slot = 0);
	void RequestHandleSound(PERAudioMessageId messageId, int slot = 0);

private:
	void ProcessPendingMessage();
	void ResizePedingArray();

	// 일회성 소리(효과음)
	virtual void PlaySound(PERSoundId id, double volume) = 0;
	virtual void StopAllSounds() = 0;

	// 배경음악
	virtual void SetBGM(PERSoundId id, double volume) = 0;
	virtual void PlayBGM() = 0;
	virtual void StopBGM() = 0;
	virtual void PauseBGM(bool isPause) = 0;

	// 환경음
	virtual void SetAmbientSound(PERSoundId id, double volume, int slot) = 0;
	virtual void PlayAmbientSound(int slot) = 0;
	virtual void StopAmbientSound(int slot) = 0;
	virtual void PauseAmbientSound(bool isPause, int slot) = 0;
	virtual void PlayAllAmbientSounds() = 0;
	virtual void StopAllAmbientSounds() = 0;
	virtual void PauseAllAmbientSounds(bool isPause) = 0;

	// 메세지 처리 대기 배열(오브젝트 생성 삭제 관련)
	int m_maxPending = PER_DEFAULT_MAX_EVENT_PENDING;
	PERAudioMessage* m_pending = new PERAudioMessage[PER_DEFAULT_MAX_EVENT_PENDING];
	int m_numPending = 0;
	// 사운드 처리 중에도 받을 수 있도록 복사해서 쓴다.
	PERAudioMessage* m_copyPending = new PERAudioMessage[PER_DEFAULT_MAX_EVENT_PENDING];
	int m_numCopyPending = 0;

	CSProvider m_csProvider;
};