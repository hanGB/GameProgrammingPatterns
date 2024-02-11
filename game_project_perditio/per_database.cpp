#include "stdafx.h"
#include "per_database.h"
#include "json_data_reader.h"

PERDatabase::PERDatabase()
{
	JSONDataReader reader;
	reader.ReadMonsterData("./data/monster_data.json", m_monsterDatas);
	reader.ReadTranslateData("./data/translate_data.json", m_translateDatas);
}

PERDatabase::~PERDatabase()
{
	for (auto& data : m_monsterDatas) {
		delete data.second;
	}
	for (auto& data : m_translateDatas) {
		delete data.second;
	}
}

MonsterData* PERDatabase::GetMonsterData(const char* id) const
{
	auto it = m_monsterDatas.find(id);
	
	if (it == m_monsterDatas.end()) {
		PERLog::Logger().ErrorWithFormat("�߸��� ���� ���̵�� �����͸� �ҷ��Խ��ϴ�: %s", id);
		// ó���� �ִ� ���� �����͸� �ӽ÷� �ѱ�
		return m_monsterDatas.begin()->second;
	}

	return it->second;
}
