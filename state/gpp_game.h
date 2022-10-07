#pragma once
#include "stdafx.h"
#include "gpp_world.h"
#include "movable_object.h"
#include "fixed_object.h"
#include "gpp_player.h"

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
		m_world->HandleInput(m_inputs);
	}
	void Update(float elapsedTimeInSec) {
		m_world->Update(elapsedTimeInSec);
	}
	void Render(HDC memDC) {
		m_world->RenderObjects(memDC, m_coordinateData);		
	}

private:
	void InitGame(HWND hWnd) {
		m_world = new GPPWorld();

		RECT rect;
		GetClientRect(hWnd, &rect);
		m_coordinateData.halfWidth = (float)rect.right / 2;
		m_coordinateData.halfHeight = (float)rect.bottom / 2;

		// 오브젝트 추가
		Ground* ground = m_world->SpawnObject<Ground>(0.0f, -1.0f, 5.0f, 0.5f, 127, 63, 0);
		ground->SetType(GPPGameObjectType::FIXED);

		GPPPlayer* player = m_world->SpawnObject<GPPPlayer>(0.0f, 0.0f, 0.8f, 0.8f, 0, 127, 127);
		player->SetType(GPPGameObjectType::PLAYER);
		player->SetGPPWorld(*m_world);
	}
	void CleanupGame() {
		delete m_world;
	}

	GPPInputChunk m_inputs;
	GPPCoordinateData m_coordinateData;
	GPPWorld* m_world;
};