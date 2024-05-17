#pragma once
#include "per_controller.h"
#include "per_renderer.h"
#include "per_audio.h"
#include "per_world.h"
#include "game_mode.h"
#include "object_storage.h"
#include "per_object.h"
#include "event_reciver.h"
#include "per_database.h"
#include "player_state.h"

class PERGame : public EventReciver {
public:
	PERGame(HWND hWnd);
	~PERGame();

	virtual void Recive(PEREvent event, PERVec3 data);

	void HandleInput(WPARAM wParam, bool isDown);

	void Update(int deltaTime);
	void UIUpdate(int deltaTime);
	void Render(HWND hWnd);
	void UIRender(HWND hWnd);

	void AudioUpdate();

	// 윈도우 hwnd 재설정
	void MatchWindowHWND(HWND hWnd);
	// 윈도우 사이즈 얻기
	void GetWindowSize(int* width, int* height);
	// 윈도우 사이즈 변경
	void ChangeWindowSize(HWND hWnd);
	// 윈도우 관련 작업
	void DoWindowJob(HWND hWnd);

private:
	// 이벤트 처리
	template <class T>
	void ProgressRunEvent(const char* worldName);
	template <class T>
	void ProgressPushAndRunEvent(const char* worldName);
	template <class T>
	void ProgressQuitAllWorldAndRunEvent(const char* worldName);
	void ProgressPopEvent();

	// 게임 월드, 모드 변경
	void Run(PERWorld* world);
	void PushWorld();
	void PopWorld();
	void Quit();
	void QuitAllWorld();

	// world관련 일 종료(월드를 변경하기 위해)
	void StopWorldJob();
	void RestartWorldJob();

	// 다음 월드에 기존 플레이어 스테이트를 넘김(플레이어의 상태를 동일하게 맞추기 위해)
	void GivePlayStateToNextWorld(PERWorld* nextWorld);

	const double c_FPS_UPDATE_GAP = 0.5;

	int m_updateLag = 0;
	std::atomic<double> m_frameGap;
	std::atomic<bool> m_isUpdateEnd = false;
	std::atomic<bool> m_isRenderEnd = false;
	// ui
	std::atomic<bool> m_isUpdateUIEnd = false;
	std::atomic<bool> m_isRenderUIEnd = false;

	PERController*	m_controller;
	PERRenderer*	m_renderer;
	PERAudio*		m_audio;
	ObjectStorage*	m_objectStorage;
	PERDatabase*	m_database;

	PERWorld*	m_currentWorld;
	std::queue<PERWorld*> m_worldQueue;
	// 월드 변경을 위해 업데이트, 렌더링을 잠시 중시 시키는 변수
	std::atomic<bool> m_isStopWorldJob = false;
	std::atomic<bool>  m_isStopUpdate = false;
	std::atomic<bool>  m_isStopUIUpdate = false;
	std::atomic<bool>  m_isStopRender = false;
	std::atomic<bool>  m_isStopUIRender = false;

	// 프레임 측정 관련
	double m_fpsUpdateTime = 0.0;
	wchar_t m_fpsText[10];
	std::atomic<int> m_fps;

	// 윈도우 관련 명령
	bool m_changeWindowSize = false;
	bool m_destroyWindow = false;
	// 창 크기 변경 관련
	bool m_isMaxScreenSize = false;
	int m_windowSizeW = PER_DEFAULT_WINDOW_WIDTH;
	int m_windowSizeH = PER_DEFAULT_WINDOW_HEIGHT;
};

template<class T>
inline void PERGame::ProgressRunEvent(const char* worldName)
{
	PERLog::Logger().InfoWithFormat("기존 월드를 삭제하고 %s 월드 실행", worldName);

	StopWorldJob();

	PERWorld* world = new T(m_objectStorage, m_database);
	GivePlayStateToNextWorld(world);
	Quit();
	Run(world);

	RestartWorldJob();
}

template<class T>
inline void PERGame::ProgressPushAndRunEvent(const char* worldName)
{
	PERLog::Logger().InfoWithFormat("기존 월드를 큐에 넣고 %s 월드 실행", worldName);

	StopWorldJob();

	PERWorld* world = new T(m_objectStorage, m_database);
	GivePlayStateToNextWorld(world);
	PushWorld();
	Run(world);

	RestartWorldJob();
}

template<class T>
inline void PERGame::ProgressQuitAllWorldAndRunEvent(const char* worldName)
{
	PERLog::Logger().InfoWithFormat("기존 월드 모두를 삭제하고 %s 월드 실행", worldName);

	StopWorldJob();

	QuitAllWorld();
	Run(new T(m_objectStorage, m_database));

	RestartWorldJob();
}
