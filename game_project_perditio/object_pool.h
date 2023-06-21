#pragma once
#include "per_object.h"
#include "object_factory.h"
#include <unordered_map>
#include <queue>

// ���� ������Ʈ
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

class ObjectPool {
public:
	ObjectPool();
	~ObjectPool();

	PERObject* PopObject(PERObjectType type);
	void PushObject(PERObjectType type, PERObject* object);

private:
	void CreateObjectFactories();
	void DeleteObjectFactories();

	void FillObjectPools();
	void RefillObjectPool(std::queue<PERObject*>& pool, PERObjectType type);
	void ClearObjectPools();

	// ������Ʈ ���丮
	std::unordered_map <PERObjectType, ObjectFactory*> m_objectFactories;
	// ������Ʈ Ǯ
	std::unordered_map<PERObjectType, std::queue<PERObject*>> m_objectPools;
};