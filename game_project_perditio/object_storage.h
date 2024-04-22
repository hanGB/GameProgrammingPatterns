#pragma once
#include "per_object.h"
#include "object_factory.h"
#include <unordered_map>
#include <queue>

class ObjectStorage {
public:
	ObjectStorage();
	~ObjectStorage();

	PERObject* PopObject(PERObjectType type);
	void PushObject(PERObjectType type, PERObject* object);

private:
	void CreateObjectFactories();
	void DeleteObjectFactories();

	void FillObjectQueues();
	void RefillObjectQueue(std::queue<PERObject*>& queue, PERObjectType type);
	void ClearObjectQueues();

	// ������Ʈ ���丮
	std::unordered_map <PERObjectType, ObjectFactory*> m_objectFactories;
	// ������Ʈ ť
	std::unordered_map<PERObjectType, std::queue<PERObject*>> m_objectQueues;
};