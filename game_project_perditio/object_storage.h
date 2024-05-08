#pragma once
#include "per_object.h"
#include "object_factory.h"
#include <unordered_map>
#include <queue>
using namespace PERComponent;

class ObjectStorage {
public:
	ObjectStorage();
	~ObjectStorage();

	PERObject* PopObject(PERObjectType type);
	void PushObject(PERObjectType type, PERObject* object);

private:
	void CreateObjectFactories();
	ObjectFactory* CreateObjectFactory(
		PERObjectType objectType, PERObjectStateType objectStateType,
		PERComponentType inputComponentType, PERComponentType aiComponenentType,
		PERComponentType physicsComponenentType, PERComponentType graphicsComponenentType,
		InputData& inputData, AiData& aiData, PhysicsData& physicsData, GraphicsData& graphicsData);
	void DeleteObjectFactories();

	void FillObjectQueues();
	void RefillObjectQueue(std::queue<PERObject*>& queue, PERObjectType type);
	void ClearObjectQueues();

	// 오브젝트 펙토리
	std::unordered_map <PERObjectType, ObjectFactory*> m_objectFactories;
	// 오브젝트 큐
	std::unordered_map<PERObjectType, std::queue<PERObject*>> m_objectQueues;
};