#pragma once
#include "stdafx.h"

namespace PERComponent {

	// ������Ʈ ����
	struct ComponentTypes {
		PERComponentType input;
		PERComponentType ai;
		PERComponentType physics;
		PERComponentType graphics;
	};

	// ������Ʈ ���� ������
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
	};

	struct GraphicsData {
		PERShapeType shape;
		PERColor color;
	};
}