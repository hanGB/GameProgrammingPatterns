#pragma once
#include <string>

#define PER_NAME_LENGTH 20

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
	char nameEng[PER_NAME_LENGTH];
	char nameKor[PER_NAME_LENGTH];
	PERStat stat;
};
