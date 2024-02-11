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
		PERLog::Logger().ErrorWithFormat("�߸��� ���� ���̵�� �����͸� �ҷ��Խ��ϴ�: %s", id);
		// ó���� �ִ� ���� �����͸� �ӽ÷� �ѱ�
		return m_MonsterDatas.begin()->second;
	}

	return it->second;
}
