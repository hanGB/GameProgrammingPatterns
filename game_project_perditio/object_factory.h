#pragma once
#include "per_object.h"
#include "per_component.h"

class ObjectFactory {
public:
	ObjectFactory();
	ObjectFactory(PERObjectType objectType, PERObjectStateType stateType, PERFloatingUiType floatingUiType,
		PERComponentType input, PERComponentType ai, 
		PERComponentType physics, PERComponentType graphics);
	~ObjectFactory();

	PERObject* CreateObject();

	PERObjectType GetObjectType() const;
	PERComponent::ComponentTypes GetComponentTypes() const;

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

private:
	void InitData();
	// 오브젝트 스테이트 생성
	ObjectState* CreateObjectState();
	// 떠있는 ui 생성
	UiElement* CreateFloatingUI();

	PERObjectType m_objectType;
	PERObjectStateType m_objectStateType;
	PERFloatingUiType m_floatingUiType;

	PERComponent::ComponentTypes m_componentTypes;

	struct ComponentData {
		PERComponent::InputData input;
		PERComponent::AiData ai;
		PERComponent::PhysicsData physics;
		PERComponent::GraphicsData graphics;
	} m_componentData;

	PERVec3 m_size;
	double m_mass;
};