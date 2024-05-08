#include "stdafx.h"
#include "per_database.h"
#include "json_data_reader.h"

PERDatabase::PERDatabase()
{
	JSONDataReader reader;
	reader.ReadMonsterData("./data/monster_data.json", m_monsterDatas);
	reader.ReadTranslateData("./data/translate_data.json", m_translateDatas);
	reader.ReadVisualData("./data/visual_data.json", m_visualDatas);
}

PERDatabase::~PERDatabase()
{
	for (auto& data : m_monsterDatas) {
		delete data.second;
	}
	for (auto& data : m_translateDatas) {
		delete data.second;
	}
	for (auto& data : m_visualDatas) {
		delete data.second;
	}
}

MonsterData* PERDatabase::GetMonsterData(const char* id) const
{
	auto it = m_monsterDatas.find(id);
	
	if (it == m_monsterDatas.end()) {
		PERLog::Logger().ErrorWithFormat("잘못된 몬스터 아이디로 데이터를 불러왔습니다: %s", id);
		// 처음에 있는 몬스터 데이터를 임시로 넘김
		return m_monsterDatas.begin()->second;
	}

	return it->second;
}

VisualData* PERDatabase::GetVisualData(const char* id) const
{
	auto it = m_visualDatas.find(id);

	if (it == m_visualDatas.cend()) {
		PERLog::Logger().ErrorWithFormat("잘못된 비주얼 아이디로 데이터를 불러왔습니다: %s", id);
		// 처음에 있는 비주얼 데이터를 임시로 넘김
		return m_visualDatas.begin()->second;
	}

	return it->second;
}

TranslateData* PERDatabase::GetTranslateData(const char* id) const
{
	auto it = m_translateDatas.find(id);

	if (it == m_translateDatas.end()) {
		PERLog::Logger().ErrorWithFormat("잘못된 번역 아이디로 데이터를 불러왔습니다: %s", id);
		// 처음에 있는 비주얼 데이터를 임시로 넘김
		return m_translateDatas.begin()->second;
	}

	return it->second;
}
