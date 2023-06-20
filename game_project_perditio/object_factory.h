#pragma once
#include "per_object.h"
#include <unordered_map>
#include <queue>

// 각종 컨포넌트
// input
#include "interact_input_component.h"
#include "no_interact_input_component.h"
// ai
#include "unintelligent_ai_component.h"
#include "intelligent_ai_component.h"
// physics
#include "movable_physics_component.h"
#include "fixed_physics_component.h"
// graphics
#include "visible_graphics_component.h"
#include "hidden_graphics_component.h"

class ObjectFactory {
public:
	ObjectFactory();
	~ObjectFactory();

	PERObject* PopObjectInPool(PERObjectType type);
	void PushObjectInPool(PERObjectType type, PERObject* object);

private:
	void FillObjectPool();
	void ClearObjectPool();

	PERObject* CreateObject(PERComponentType inputType, PERComponentType aiType, PERComponentType physicsType, PERComponentType graphicsType);

	// 오브젝트 풀
	std::unordered_map<PERObjectType, std::queue<PERObject*>> m_objectPools;
};