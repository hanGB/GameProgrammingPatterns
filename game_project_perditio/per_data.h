#pragma once
#include <string>

enum class PERShapeType;
enum class PERBoundingType;

struct PERStat {
	short level = 1;
	short body = 0;
	short mind = 0;
	short physicalAttack = 0;
	short physicalDefense = 0;
	short mindAttack = 0;
	short mindDefense = 0;
};

struct MonsterData {
	std::string nameId;
	std::string visualId;
	PERStat stat;
};

struct TranslateData {
	std::wstring engUS;
	std::wstring korKR;
};

struct VisualData {
	PERShapeType shape;
	PERVec3 size;
	int mass;
	PERColor color;
	bool borderOn;
	int borderWidth;
	PERColor borderColor;
	PERBoundingType boundingType;
};

struct EffectData {
	PERShapeType shape;
	PERVec3 size;
	int amount;
	double speedRate;
	double spawnDelay;
	double particleLifeTime;
	PERColor color;
	bool borderOn;
	int borderWidth;
	PERColor borderColor;
};

enum class PERDatabaseType {
	MONSTER,
	TRANSLATE,
	VISUAL,
	EFFECT,
	NUM_DATABASE_TYPE
};
