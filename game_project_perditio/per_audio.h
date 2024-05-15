#pragma once

enum class PERSoundId {
	// short sound
	BLADE_SWING,
	BULLET_SHOOTING,
	SWITCH_USING,

	// BGM
	BATTLE_BGM,

	// num
	NUM_SOUND_ID
};

enum class PERAudioMessageId {
	PLAY_SOUND,
	STOP_SOUND,
	STOP_ALL_SOUNDS,
	SET_BGM,
	PLAY_CURRENT_BGM,
	STOP_CURRENT_BGM,
	PAUSE_CURRENT_BGM,
	RESUME_CURRENT_BGM,
	ADD_BGM,
	STOP_ALL_BGM,
	NUM_WROLD_MESSAGE_ID
};

struct PERAudioMessage {
	PERAudioMessageId messageId;
	PERSoundId soundId;
	double volume;
};

class PERAudio {
public:
	PERAudio();
	~PERAudio();

	virtual void Update();

	virtual void LoadSound(PERSoundId id, const char* fileName) = 0;
	void RequestHandleSound(PERAudioMessageId messageId, PERSoundId soundId, double volume);

private:
	void ProcessPendingMessage();
	void ResizePedingArray();

	virtual void PlaySound(PERSoundId id, double volume) = 0;
	virtual void StopSound(PERSoundId id) = 0;
	virtual void StopAllSounds() = 0;

	virtual void SetBGM(PERSoundId id, double volume) = 0;
	virtual void AddBGM(PERSoundId id, double volume) = 0;
	virtual void PlayCurrentBGM() = 0;
	virtual void StopCurrentBGM() = 0;
	virtual void PauseCurrentBGM(bool isPause) = 0;
	virtual void StopAllBGMs() = 0;

	// 메세지 처리 대기 배열(오브젝트 생성 삭제 관련)
	int m_maxPending = PER_DEFAULT_MAX_EVENT_PENDING;
	PERAudioMessage* m_pending = new PERAudioMessage[PER_DEFAULT_MAX_EVENT_PENDING];
	int m_numPending = 0;
};