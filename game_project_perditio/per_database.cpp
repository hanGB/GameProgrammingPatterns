#include "stdafx.h"
#include "per_database.h"
#include "json_data_reader.h"

PERDatabase::PERDatabase()
{
	JSONDataReader reader;
	reader.ReadMonsterData("./data/monster_data.json", m_MonsterDatas);
}

PERDatabase::~PERDatabase()
{
	for (auto& MonsterData : m_MonsterDatas) {
		delete MonsterData.second;
	}
}

MonsterData* PERDatabase::GetMonsterData(const char* id) const
{
	auto it = m_MonsterDatas.find(id);
	
	if (it == m_MonsterDatas.end()) {
		PERLog::Logger().ErrorWithFormat("잘못된 몬스터 아이디로 데이터를 불러왔습니다: %s", id);
		// 처음에 있는 몬스터 데이터를 임시로 넘김
		return m_MonsterDatas.begin()->second;
	}

	return it->second;
}
