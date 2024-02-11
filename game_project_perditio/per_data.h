#pragma once
#include <string>

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
	PERStat stat;
};

struct TranslateData {
	std::string engUS;
	std::string korKR;
};
