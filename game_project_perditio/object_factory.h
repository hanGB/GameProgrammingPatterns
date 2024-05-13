#pragma once
#include "per_object.h"
#include "per_component.h"

class ObjectFactory {
public:
	ObjectFactory(PERObjectType objectType, PERObjectStateType objectStateType,
		std::vector<PERComponentType>& input, std::vector<PERComponentType>& ai,
		std::vector<PERComponentType>& physics, std::vector<PERComponentType>& graphics);
	ObjectFactory(PERObjectType objectType, PERObjectStateType objectStateType,
		std::vector<PERComponentType>& input, std::vector<PERComponentType>& ai,
		std::vector<PERComponentType>& physics, std::vector<PERComponentType>& graphics,
		PERComponent::InputData& inputData, PERComponent::AiData& aiData, 
		PERComponent::PhysicsData& physicsData, PERComponent::GraphicsData& graphicsData);
	~ObjectFactory();

	PERObject* CreateObject();

	PERObjectType GetObjectType() const;
	PERCollisionType GetCollisionType() const;
	PERComponent::ComponentTypeVectors& GetComponentTypeVectors();

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

	InputComponent* CreateInputComponent(std::vector<PERComponentType>& types);
	AiComponent* CreateAiComponent(std::vector<PERComponentType>& types);
	PhysicsComponent* CreatePhysicsComponent(std::vector<PERComponentType>& types);
	GraphicsComponent* CreateGraphicsComponent(std::vector<PERComponentType>& types);

	InputComponent* CreateInputComponent(PERComponentType type);
	AiComponent* CreateAiComponent(PERComponentType type);
	PhysicsComponent* CreatePhysicsComponent(PERComponentType type);
	GraphicsComponent* CreateGraphicsComponent(PERComponentType type);
	
	void MatchCollisionType();
	
	PERObjectType m_objectType;
	PERObjectStateType m_objectStateType;
	PERCollisionType m_collisionType;

	PERComponent::ComponentTypeVectors m_componentTypeVectors;

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