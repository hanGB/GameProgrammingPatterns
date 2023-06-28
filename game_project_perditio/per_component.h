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
		bool isMove;
		bool isAttack;
		bool isCheck;
	};

	struct AiData {
		bool isMove;
		bool isAttack;
	};

	struct PhysicsData {
		bool friction;
		bool isOccupySpace;
	};

	struct GraphicsData {
		PERShapeType shape;
		PERColor color;
		bool border;
		int borderWidth;
		PERColor borderColor;
	};
}