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
	};

	struct PhysicsData {
		bool friction = true;
		bool isOccupySpace = true;
	};

	struct GraphicsData {
		PERShapeType shape = PERShapeType::RECTANGLE;
		PERColor color = PERColor(255, 255, 255);
		bool border = true;
		int borderWidth = 1;
		PERColor borderColor = PERColor(0, 0, 0);
		bool floatingUi = false;
		double distanceVisiblefloatingUi = 0.0;
	};
}