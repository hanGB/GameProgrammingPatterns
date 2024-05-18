#pragma once
#include "stdafx.h"

class PERAudio;
class ObjectStorage;
class PERController;
class PERRenderer;
class PERObject;
class GameMode;
class PERDatabase;
class PERHud;
class PERParticlePool;
class InputComponent;
class AiComponent;
class PhysicsComponent;
class GraphicsComponent;
enum class PERObjectType;
enum class PERCollisionType;

enum class PERWorldMessageId {
	ADD_OBJECT,
	DELETE_OBJECT,
	SLEEP_OBJECT,
	WAKE_UP_OBJECT,
	NUM_WROLD_MESSAGE_ID
};

enum class PERWorldType {
	MAIN_MENU,
	NONE,
	TEST_WORLD,
	TEST_WORLD2,
	NUM_WORLD_TYPE
};

struct PERWorldMessage {
	PERWorldMessageId id;
	PERObject* object;
	PERObjectType type;
	std::string databaseId;
	PERDatabaseType databaseType;
	PERVec3 position;
	PERVec3 currentAccel;
	PERStat stat;
	double lifeTime;
};

class PERWorld {
public:
	PERWorld();
	~PERWorld();

	void Update(PERAudio& audio, double dTime);
	void UIUpdate(PERController& controller, PERAudio& audio, double dTime);

	void InputUpdate(PERController& controller, PERAudio& audio, double dTime);
	void AiUpdate(PERAudio& audio, double dTime);
	void PhysicsUpdate(PERAudio& audio, double dTime);
	void GraphicsUpdate(PERAudio& audio, double dTime);

	void UpdateSortedGraphicsComponents();
	void UpdateCamera(PERRenderer& renderer, double frameGap);

	void Render(PERRenderer& renderer, double frameGap);
	void UIRender(PERRenderer& renderer);

	virtual void Enter(PERRenderer& renderer, PERAudio& audio);
	virtual void Exit(PERRenderer& renderer, PERAudio& audio);
	virtual void Pause(PERRenderer& renderer, PERAudio& audio);
	virtual void Resume(PERRenderer& renderer, PERAudio& audio);

	// 요청을 받는 함수
	void RequestAddObject(
		PERObject* parent, PERObjectType type, const char* databaseId, PERDatabaseType databaseType, PERStat stat,
		PERVec3 position, double lifeTime = PER_MAXIMUM_LIFE_TIME, PERVec3 currentAccel = PERVec3(0.0, 0.0, 0.0));
	void RequestDeleteObject(PERObject* object);

	// 충돌 확인
	bool CheckCollision(PERObject& object, PERAudio& audio, double dTime);
	// 오브젝트를 하나 제외하고 충돌 체크함
	bool CheckCollisionWithoutSpecificObject(PERObject& object, PERObject& exceptObject, PERAudio& audio, double dTime);

	PERHud& GetHud();
	PERDatabase& GetDatabase();
	PERParticlePool& GetParticlePool();
	GameMode& GetGameMode();
	
protected:
	// 자식이 접근해서 사용할 함수
	// 월드 설정
	void InitSettingForWorld(ObjectStorage* objectStorage, PERDatabase* database, GameMode* mode);

	// 오브젝트 비주얼 설정
	void SetObjectVisual(PERObject* object, const char* visualId);

	// 오브젝트 색상 모양 설정
	void SetObjectShapeAndColor(PERObject* object, PERShapeType shape, PERColor color,
		bool border = true, int borderWidth = 1, PERColor borderColor = PERColor(0, 0, 0));

	// 오브젝트 추가 삭제 관련
	PERObject* AddAndGetObject(PERObjectType type);
	void AddObject(PERObject* object);
	void DeleteObject(PERObject* object);

	int GetNumObject() const;
	std::vector<PERObject*>& GetObjects();

	// 맵 데이터 만들기
	void MakeFixedObjectsDataToFile(const char* fileName);
	void ReadFixedObjectsDataFromFile(const char* fileName);

	ObjectStorage* m_objectStorage;
	PERDatabase* m_database;
	GameMode* m_gameMode;

private:
	virtual void InitWorldObject() = 0;

	void DoGarbegeCollection(double dTime);
	void InformObjectDeadToSpawner(PERObject* object);
	void SleepAndWakeupObjects();
	void ProcessPendingMessage();

	void RequestSimpleDoObject(PERObject* object, const PERWorldMessageId& message);

	// Add Message  처리
	void ProcessAddMessage(PERWorldMessage& message);
	void SetBaseOfAddMessage(PERWorldMessage& message, PERObject* newObject, VisualData* vData);
	void SetForAddParticleEffecterMessage(PERWorldMessage& message, PERObject* newObject, EffectData* eData);
	void SetForAddByObjectMessage(PERWorldMessage& message, PERObject* newObject);

	// 오브젝트 잠들게 함
	void SleepObject(PERObject* object);
	// 오브젝트 깨움
	void WakeUpObject(PERObject* object);

	void ResizePedingArray();

	// 반환
	void ReturnObejctToStorage();
	void ClearObejctVector();

	std::vector<PERObject*> m_objects;
	int m_maxObject = PER_DEFAULT_MAX_OBJECTS;
	int m_numObject = 0;
	// 컨포넌트
	std::vector<InputComponent*> m_inputComponents;
	std::vector<AiComponent*> m_aiComponents;
	std::vector<PhysicsComponent*> m_physicsComponents;
	std::vector<GraphicsComponent*> m_graphicsComponents;

	// 렌더링용 z좌표로 정렬된 그래픽스 컨포넌트
	std::vector<GraphicsComponent*> m_sortedGraphicsComponents;
	bool m_isUpdateSortedGraphicsComponent = false;

	// 슬립 오브젝트 보관
	std::vector<PERObject*> m_sleepObjects;
	int m_maxSleepObject = PER_DEFAULT_MAX_OBJECTS;
	int m_numSleepObject = 0;

	// 이펙트용 파티클 풀
	PERParticlePool* m_particlePool;

	// 메세지 처리 대기 배열(오브젝트 생성 삭제 관련)
	int m_maxPending = PER_DEFAULT_MAX_EVENT_PENDING;
	PERWorldMessage* m_pending = new PERWorldMessage[PER_DEFAULT_MAX_EVENT_PENDING];
	int m_numPending = 0;
};