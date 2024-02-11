#pragma once
#include "per_data.h"

class PERDatabase {
public:
	PERDatabase();
	~PERDatabase();

	MonsterData* GetMonsterData(const char* id) const;

private:
	std::map<std::string, MonsterData*> m_MonsterDatas;
};