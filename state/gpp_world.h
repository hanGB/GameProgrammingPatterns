#pragma once
#include "stdafx.h"
#include "gpp_game_object.h"

class GPPWorld {
public:
	GPPWorld() {  }
	~GPPWorld() { RemoveAllObjects(); }
	
	void HandleInput(GPPInputChunk& inputs) {
		for (auto object : m_objectList) {
			object->HandleInput(inputs);
		}
	}
	void Update(float elapsedTimeInSec) {
		DoGarbegeCollection(elapsedTimeInSec);

		for (auto object : m_objectList) {
			object->Update(elapsedTimeInSec);
		}
		CheckCollision();
	}
	void RenderObjects(HDC& memDC, GPPCoordinateData& coordinateData) {

		for (auto object : m_objectList) {
			object->Render(memDC, coordinateData);
		}
	}

	template <class T>
	T* SpawnObject(float positionX, float positionY, float sizeX, float sizeY, int red, int green, int blue) {
		m_objectList.emplace_back(new T(positionX, positionY, sizeX, sizeY, red, green, blue));
		auto it = m_objectList.end();
		it--;
		GPPGameObject* object = *it;

		return dynamic_cast<T*>(object);
	}

private:
	void DoGarbegeCollection(float elapsedTime) {
		auto it = m_objectList.begin();
		while (it != m_objectList.end()) {
			if ((*it)->GetIsUseLifeTime()) {
				float lifeTime = (*it)->GetLifeTime();
				if (lifeTime < 0.0f) {
					delete (*it);
					m_objectList.erase(it++);
					continue;
				}
				(*it)->SetLifeTime(lifeTime - elapsedTime);
			}
			it++;
		}
	}
	void RemoveAllObjects() {

		auto it = m_objectList.begin();
		while (it != m_objectList.end()) {
			delete (*it);
			m_objectList.erase(it++);
			continue;
			it++;
		}
		m_objectList.clear();
	}
	void CheckCollision() {

		for (auto objectAIter = m_objectList.begin(); objectAIter != m_objectList.end(); ++objectAIter) {
			auto objectBIter = objectAIter;
			objectBIter++;

			for (objectBIter; objectBIter != m_objectList.end(); ++objectBIter)
			{
				GPPGameObjectType aType = (*objectAIter)->GetType();
				GPPGameObjectType bType = (*objectBIter)->GetType();
				if (aType == GPPGameObjectType::FIXED && bType == GPPGameObjectType::FIXED) {
					continue;
				}
				else {
					if (IsCollidedWithAABB(**objectAIter, **objectBIter)) {
						AdjustPosition(**objectAIter, **objectBIter);
						(*objectAIter)->LandOnGround();
						(*objectBIter)->LandOnGround();
						(*objectAIter)->SetIsCollided(true);
						(*objectBIter)->SetIsCollided(true);
					}
				}
			}
		}

		// 아무것도 충돌하지 않은 경우 떨어지도록 설정
		for (auto object : m_objectList) {
			if (!object->GetIsCollided())
				object->Fall();
			object->SetIsCollided(false);
		}
	}
	bool IsCollidedWithAABB(GPPGameObject& objectA, GPPGameObject& objectB) {
		float aPosX, aPosY;
		objectA.GetPosition(&aPosX, &aPosY);
		float aHalfSizeX, aHalfSizeY;
		objectA.GetHalfSize(&aHalfSizeX, &aHalfSizeY);
		float bPosX, bPosY;
		objectB.GetPosition(&bPosX, &bPosY);
		float bHalfSizeX, bHalfSizeY;
		objectB.GetHalfSize(&bHalfSizeX, &bHalfSizeY);

		if (aPosX + aHalfSizeX < bPosX - bHalfSizeX) return false;
		if (aPosX - aHalfSizeX > bPosX + bHalfSizeX) return false;
		if (aPosY + aHalfSizeY < bPosY - bHalfSizeY) return false;
		if (aPosY - aHalfSizeY > bPosY + bHalfSizeY) return false;
		return true;
	}
	void AdjustPosition(GPPGameObject& objectA, GPPGameObject& objectB) {
		GPPGameObjectType aType = objectA.GetType();
		GPPGameObjectType bType = objectB.GetType();

		if (aType == GPPGameObjectType::PLAYER || aType == GPPGameObjectType::MOVABLE) {
			if (bType == GPPGameObjectType::FIXED) {
				MoveOneObjecttoAdjustPosition(objectA, objectB);
			}
		}
		else if (bType == GPPGameObjectType::PLAYER || bType == GPPGameObjectType::MOVABLE) {
			if (aType == GPPGameObjectType::FIXED) {
				MoveOneObjecttoAdjustPosition(objectB, objectA);
			}
		}

	}
	void MoveOneObjecttoAdjustPosition(GPPGameObject& movableObject, GPPGameObject& fixedObject) {
		float mPosX, mPosY;
		movableObject.GetPosition(&mPosX, &mPosY);
		float mHalfSizeX, mHalfSizeY;
		movableObject.GetHalfSize(&mHalfSizeX, &mHalfSizeY);
		float fPosX, fPosY;
		fixedObject.GetPosition(&fPosX, &fPosY);
		float fHalfSizeX, fHalfSizeY;
		fixedObject.GetHalfSize(&fHalfSizeX, &fHalfSizeY);

		if (fPosY + fHalfSizeY > mPosY - mHalfSizeY) {
			mPosY = mPosY + (fPosY + fHalfSizeY - (mPosY - mHalfSizeY));

			movableObject.SetPosition(mPosX, mPosY);
			movableObject.SetState(GPPGameObjectState::STANDING);
		}
	}

	std::list<GPPGameObject*> m_objectList;
};