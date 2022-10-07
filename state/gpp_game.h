#pragma once
#include "stdafx.h"
#include "gpp_game_object.h"
#include "movable_object.h"
#include "fixed_object.h"

class GPPGame {
public:
	GPPGame(HWND hWnd) { InitGame(hWnd); }
	~GPPGame() { CleanupGame(); }
	
	void HandleKeyboardInput(bool isDown, WPARAM wParam) {

		if (wParam == 'w' || wParam == 'W') m_inputs.Input_W = isDown;
		if (wParam == 'a' || wParam == 'A') m_inputs.Input_A = isDown;
		if (wParam == 's' || wParam == 'S') m_inputs.Input_S = isDown;
		if (wParam == 'd' || wParam == 'D') m_inputs.Input_D = isDown;

		if (wParam == VK_SPACE)	m_inputs.Input_Space = isDown;
	}

	void HandleInput() {

		for (size_t i = 0; i < m_objectCount; ++i) {
			m_objects[i]->HandleInput(m_inputs);
		}
	}
	void Update(float elapsedTimeInSec) {
		for (size_t i = 0; i < m_objectCount; ++i) {
			m_objects[i]->Update(elapsedTimeInSec);
		}
		CheckCollision();
	}
	void Render(HDC memDC) {

		for (size_t i = 0; i < m_objectCount; ++i) {
			m_objects[i]->Render(memDC, m_coordinateData);
		}
	}

private:
	void InitGame(HWND hWnd) {

		RECT rect;
		GetClientRect(hWnd, &rect);
		m_coordinateData.halfWidth = (float)rect.right / 2;
		m_coordinateData.halfHeight = (float)rect.bottom / 2;

		// 오브젝트 추가
		m_objects[m_objectCount] = new Ground(0.0f, -1.0f, 5.0f, 0.5f);
		m_objects[m_objectCount]->SetType(GPPGameObjectType::FIXED);
		m_objectCount++;

		m_objects[m_objectCount] = new Player(0.0f, 0.0f);
		m_objects[m_objectCount]->SetType(GPPGameObjectType::PLAYER);
		m_objectCount++;
	}
	void CleanupGame() {

		for (size_t i = 0; i < m_objectCount; ++i) {
			delete m_objects[i];
		}
	}
	void CheckCollision() {
		memset(collided, 0, sizeof(collided));

		for (size_t i = 0; i < m_objectCount; ++i) {
			for (size_t t = i; t < m_objectCount; ++t) {
				// 자기 자신과는 충돌검사를 하지 않음
				if (m_objects[i] == m_objects[t]) continue;

				GPPGameObjectType aType = m_objects[i]->GetType();
				GPPGameObjectType bType = m_objects[t]->GetType();

				if (aType == GPPGameObjectType::FIXED && bType == GPPGameObjectType::FIXED) {
					continue;
				}
				else {
					if (IsCollidedWithAABB(*m_objects[i], *m_objects[t])) {
						AdjustPosition(*m_objects[i], *m_objects[t]);
						m_objects[i]->LandOnGround();
						m_objects[t]->LandOnGround();
						collided[i] = true;
						collided[t] = true;
					}
				}
			}
		}

		// 아무것도 충돌하지 않은 경우 떨어지도록 설정
		for (size_t i = 0; i < m_objectCount; ++i) {
			if (!collided[i])
				m_objects[i]->Fall();
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
		else if(bType == GPPGameObjectType::PLAYER || bType == GPPGameObjectType::MOVABLE) {
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


	GPPInputChunk m_inputs;
	GPPCoordinateData m_coordinateData;
	GPPGameObject* m_objects[MAXIMUM_OBJECT_COUNT];
	int m_objectCount = 0;
	bool collided[MAXIMUM_OBJECT_COUNT];
};