#pragma once
#include "per_object.h"
#include "per_component.h"

class ObjectFactory {
public:
	ObjectFactory();
	ObjectFactory(PERObjectType objectType, PERObjectStateType objectStateType,
		PERComponentType input, PERComponentType ai, 
		PERComponentType physics, PERComponentType graphics);
	ObjectFactory(PERObjectType objectType, PERObjectStateType objectStateType,
		PERComponentType input, PERComponentType ai,
		PERComponentType physics, PERComponentType graphics, 
		PERComponent::InputData& inputData, PERComponent::AiData& aiData, 
		PERComponent::PhysicsData& physicsData, PERComponent::GraphicsData& graphicsData);
	~ObjectFactory();

	PERObject* CreateObject();

	PERObjectType GetObjectType() const;
	PERComponent::ComponentTypes GetComponentTypes() const;

	// 스태틱 함수(object storage에서도 컨포넌트 데이터들 초기화하기 위함)
	static void InitComponentDatas(PERComponent::InputData& input, PERComponent::AiData& ai,
	PERComponent::PhysicsData& physics, PERComponent::GraphicsData& graphics);

	// setter
	void SetInputData(PERComponent::InputData input);
	void SetAiData(PERComponent::AiData ai);
	void SetPhysicsData(PERComponent::PhysicsData physics);
	void SetGraphicsData(PERComponent::GraphicsData graphics);

	void SetSize(PERVec3 size);
	void SetMass(double mass);

	// gettter
	PERComponent::InputData GetInputData() const;
	PERComponent::AiData GetAiData() const;
	PERComponent::PhysicsData GetPhysicsData() const;
	PERComponent::GraphicsData GetGraphicsData() const;

	PERVec3 GetSize() const;
	double GetMass() const;

	// 오브젝트 스테이트 스텟 초기화
	void InitializeObjectState(ObjectState* objectState);

private:
	void InitData();
	// 오브젝트 스테이트 생성
	ObjectState* CreateObjectState();
	
	PERObjectType m_objectType;
	PERObjectStateType m_objectStateType;

	PERComponent::ComponentTypes m_componentTypes;

	struct ComponentData {
		PERComponent::InputData input;
		PERComponent::AiData ai;
		PERComponent::PhysicsData physics;
		PERComponent::GraphicsData graphics;
	} m_componentData;

	PERVec3 m_size;
	double m_mass;

	PERStat m_stat;
};