#pragma once
#include "stdafx.h"

namespace PERComponent {

	// 컨포넌트 종류
	struct ComponentTypes {
		PERComponentType input;
		PERComponentType ai;
		PERComponentType physics;
		PERComponentType graphics;
	};

	// 컨포넌트 설정 데이터
	struct InputData {
		bool isMove = false;
		bool isAttack = false;
		bool isCheck = false;
	};

	struct AiData {
		bool isMove = false;
		bool isAttack = false;
		bool isSwitch = false;
		bool isDisposable = false;
		double particleDelay = 0.5;
		double particleLifeTime = 3.0;
		double particleSpeed = 1.0;
		PERParticleEffectType particleEffectType = PERParticleEffectType::CIRCLE_BOMB;
	};

	struct PhysicsData {
		bool friction = true;
		bool isOccupySpace = true;
		PERVec3 stuckPosition = PERVec3(0.0, 0.0, 0.0);
	};

	struct GraphicsData {
		PERShapeType shape = PERShapeType::RECTANGLE;
		PERColor color = PERColor(255, 255, 255);
		bool border = true;
		int borderWidth = 1;
		PERColor borderColor = PERColor(0, 0, 0);
	};
}