#pragma once
#include "stdafx.h"
#include "per_renderer.h"

enum class PERComponentType {
	// input
	PLAYER_INPUT,
	INTERACT,
	NO_INTERACT,
	BUTTON_INPUT,
	MENU_SELECT,

	// ai
	UNINTELLIGENT,
	INTELLIGENT,
	MONSTER_AI,
	SPAWNER_AI,
	RESPONSE_TO_SIGNAL,
	MAKING_SIGNAL,
	CREATING_PARTICLE,

	// physics
	FIXED,
	MOVABLE,
	STUCK,
	PRESSURE_PHYSICS,
	TRIGGER_PHYSICS,

	// graphics
	VISIBLE,
	NAME_TAG_GRAPHICS,
	BODY_BAR_GRAPHICS,
	KEY_INPUT_HELPER_GRAPHICS,
	TEXT_GRAPHICS,
	HIDDEN,
	HIDDEN_DEBUG,

	NUM_COMPONENT_TYPE
};

enum class PERParticleEffectType {
	CIRCLE_BOMB,
	POWER_ABSORPTION,
	NUM_PARTICLE_EFFECT_TYPE
};

namespace PERComponent {

	// 컨포넌트 종류
	struct ComponentTypeVectors {
		std::vector<PERComponentType> input;
		std::vector<PERComponentType> ai;
		std::vector<PERComponentType> physics;
		std::vector<PERComponentType> graphics;
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