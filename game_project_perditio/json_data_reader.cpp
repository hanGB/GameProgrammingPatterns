#include "stdafx.h"
#include "json_data_reader.h"

JSONDataReader::JSONDataReader( )
{

}

JSONDataReader::~JSONDataReader( )
{
	
}

void JSONDataReader::ReadMonsterData(const char* fileName, std::map<std::string, MonsterData*>& dataMap)
{
	ReadJson(fileName);

	dataMap.emplace("MONSTER_KOPPER", MakeMonsterData("MONSTER_KOPPER"));
	dataMap.emplace("MONSTER_NIKKEL", MakeMonsterData("MONSTER_NIKKEL"));
	dataMap.emplace("MONSTER_ANTIMONY", MakeMonsterData("MONSTER_ANTIMONY"));

	m_document.RemoveAllMembers();
}

void JSONDataReader::ReadJson(const char* fileName)
{
	char* buffer = new char[c_BUFFER_SIZE];

	// 파일 읽기
	FILE* file;
	fopen_s(&file, fileName, "rb");
	rapidjson::FileReadStream is(file, buffer, c_BUFFER_SIZE);
	fclose(file);

	// json 파싱
	m_document.ParseStream(is);

	delete[] buffer;
}

MonsterData* JSONDataReader::MakeMonsterData(const char* monsterId)
{
	MonsterData* data = new MonsterData;

	// 잘못된 아이디인 경우의 오류 검출 필요

	strcpy_s(data->nameEng, m_document[monsterId]["NAME_ENG"].GetString());
	strcpy_s(data->nameKor, m_document[monsterId]["NAME_KOR"].GetString());
	data->stat.level = m_document[monsterId]["LEVEL"].GetInt();
	data->stat.body = m_document[monsterId]["BODY"].GetInt();
	data->stat.mind = m_document[monsterId]["MIND"].GetInt();
	data->stat.physicalAttack = m_document[monsterId]["PHYSICAL_ATTACK"].GetInt();
	data->stat.mindAttack = m_document[monsterId]["MIND_ATTACK"].GetInt();
	data->stat.physicalDefense = m_document[monsterId]["PHYSICAL_DEFENSE"].GetInt();
	data->stat.mindDefense = m_document[monsterId]["MIND_DEFENSE"].GetInt();

	/*PERLog::Logger().InfoWithFormat("%s - \n nameEng: %s, nameKor: %s \n level: %d body: %d mind %d \n a: %d %d d: %d %d",
		monsterId, data->nameEng, data->nameKor, data->stat.level, data->stat.body, data->stat.mind,
		data->stat.physicalAttack, data->stat.mindAttack, data->stat.physicalDefense, data->stat.mindDefense);*/

	return data;
}
