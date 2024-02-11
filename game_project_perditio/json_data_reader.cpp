#include "stdafx.h"
#include "json_data_reader.h"

JSONDataReader::JSONDataReader()
{

}

JSONDataReader::~JSONDataReader()
{

}

void JSONDataReader::ReadMonsterData(const char* fileName, std::map<std::string, MonsterData*>& dataMap)
{
	ReadJson(fileName);

	for (auto it = m_document.MemberBegin(); it != m_document.MemberEnd(); ++it) {
		const char* id = it->name.GetString();
		dataMap.emplace(id, MakeMonsterData(id));
	}

	m_document.RemoveAllMembers();
}

void JSONDataReader::ReadTranslateData(const char* fileName, std::map<std::string, TranslateData*>& dataMap)
{
	ReadJson(fileName);

	for (auto it = m_document.MemberBegin(); it != m_document.MemberEnd(); ++it) {
		const char* id = it->name.GetString();
		dataMap.emplace(id, MakeTranslateData(id));
	}

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

MonsterData* JSONDataReader::MakeMonsterData(const char* id)
{
	MonsterData* data = new MonsterData;

	// 잘못된 아이디인 경우 오류 출력
	if (!m_document.HasMember(id)) {
		PERLog::Logger().ErrorWithFormat("Not correct monster ID: %s", id);
		return nullptr;
	}

	data->nameId = m_document[id]["NAME_ID"].GetString();
	data->stat.level = m_document[id]["LEVEL"].GetInt();
	data->stat.body = m_document[id]["BODY"].GetInt();
	data->stat.mind = m_document[id]["MIND"].GetInt();
	data->stat.physicalAttack = m_document[id]["PHYSICAL_ATTACK"].GetInt();
	data->stat.mindAttack = m_document[id]["MIND_ATTACK"].GetInt();
	data->stat.physicalDefense = m_document[id]["PHYSICAL_DEFENSE"].GetInt();
	data->stat.mindDefense = m_document[id]["MIND_DEFENSE"].GetInt();

	return data;
}

TranslateData* JSONDataReader::MakeTranslateData(const char* id)
{
	TranslateData* data = new TranslateData;

	// 잘못된 아이디인 경우 오류 출력
	if (!m_document.HasMember(id)) {
		PERLog::Logger().ErrorWithFormat("Not correct translate ID: %s", id);
		return nullptr;
	}

	data->engUS = m_document[id]["ENG_US"].GetString();
	data->korKR = m_document[id]["KOR_KR"].GetString();

	return data;
}
