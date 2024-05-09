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

void JSONDataReader::ReadVisualData(const char* fileName, std::map<std::string, VisualData*>& dataMap)
{
	ReadJson(fileName);

	for (auto it = m_document.MemberBegin(); it != m_document.MemberEnd(); ++it) {
		const char* id = it->name.GetString();
		dataMap.emplace(id, MakeVisualData(id));
	}

	m_document.RemoveAllMembers();
}

void JSONDataReader::ReadEffectData(const char* fileName, std::map<std::string, EffectData*>& dataMap)
{
	ReadJson(fileName);

	for ( auto it = m_document.MemberBegin(); it != m_document.MemberEnd(); ++it ) {
		const char* id = it->name.GetString();
		dataMap.emplace(id, MakeEffectData(id));
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

void JSONDataReader::UTF8TextToWString(const char* utf8Text, std::wstring& result)
{
	int nLen = MultiByteToWideChar(CP_UTF8, 0, utf8Text, sizeof(utf8Text), NULL, NULL);
	MultiByteToWideChar(CP_UTF8, 0, utf8Text, sizeof(utf8Text), &result[0], nLen);
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
	data->visualId = m_document[id]["VISUAL_ID"].GetString();
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
	UTF8TextToWString(m_document[id]["ENG_US"].GetString(), data->engUS);
	UTF8TextToWString(m_document[id]["KOR_KR"].GetString(), data->korKR);

	return data;
}

VisualData* JSONDataReader::MakeVisualData(const char* id)
{
	VisualData* data = new VisualData;

	// 잘못된 아이디인 경우 오류 출력
	if (!m_document.HasMember(id)) {
		PERLog::Logger().ErrorWithFormat("Not correct visual ID: %s", id);
		return nullptr;
	}

	data->size = 
		PERVec3(
			m_document[id]["SIZE_X"].GetDouble(), 
			m_document[id]["SIZE_Y"].GetDouble(), 
			m_document[id]["SIZE_Z"].GetDouble()
		);
	data->mass = m_document[id]["MASS"].GetInt();
	data->color =
		PERColor(
			m_document[id]["COLOR_R"].GetInt(),
			m_document[id]["COLOR_G"].GetInt(),
			m_document[id]["COLOR_B"].GetInt()
		);
	data->borderOn = m_document[id]["BORDER_ON"].GetInt();
	data->borderWidth = m_document[id]["BORDER_WIDTH"].GetInt();
	data->borderColor =
		PERColor(
			m_document[id]["BORDER_COLOR_R"].GetInt(),
			m_document[id]["BORDER_COLOR_G"].GetInt(),
			m_document[id]["BORDER_COLOR_B"].GetInt()
		);

	// enum 데이터 설정
	std::string shape = m_document[id]["SHAPE"].GetString();
	if (strcmp(shape.c_str(), "ellipse") == 0) data->shape = PERShapeType::ELLIPSE;
	else if (strcmp(shape.c_str(), "triangle") == 0) data->shape = PERShapeType::TRIANGLE;
	else if (strcmp(shape.c_str(), "rectangle") == 0) data->shape = PERShapeType::RECTANGLE;
	else if (strcmp(shape.c_str(), "round rectangle") == 0) data->shape = PERShapeType::ROUND_RECTANGLE;
	else {
		PERLog::Logger().ErrorWithFormat("Not correct shape: %s", shape);
		data->shape = PERShapeType::RECTANGLE;
	}

	std::string boundingType = m_document[id]["BOUNDING_TYPE"].GetString();
	if (strcmp(boundingType.c_str(), "rectangle") == 0) data->boundingType = PERBoundingType::RECTANGLE;
	else if (strcmp(boundingType.c_str(), "circle") == 0) data->boundingType = PERBoundingType::CIRCLE;
	else if (strcmp(boundingType.c_str(), "triangle") == 0) data->boundingType = PERBoundingType::TRIANGLE;
	else {
		PERLog::Logger().ErrorWithFormat("Not correct bounding type: %s", boundingType);
		data->boundingType = PERBoundingType::RECTANGLE;
	}

	return data;
}

EffectData* JSONDataReader::MakeEffectData(const char* id)
{
	EffectData* data = new EffectData;

	// 잘못된 아이디인 경우 오류 출력
	if ( !m_document.HasMember(id) ) {
		PERLog::Logger().ErrorWithFormat("Not correct visual ID: %s", id);
		return nullptr;
	}

	data->size =
		PERVec3(
			m_document[id]["SIZE_X"].GetDouble(),
			m_document[id]["SIZE_Y"].GetDouble(),
			m_document[id]["SIZE_Z"].GetDouble()
		);
	data->amount = m_document[ id ][ "AMOUNT" ].GetInt();
	data->speedRate = m_document[ id ][ "SPEED_RATE" ].GetDouble();
	data->spawnDelay = m_document[ id ][ "SPAWN_DELAY" ].GetDouble();
	data->particleLifeTime = m_document[ id ][ "PARTICLE_LIFE_TIME" ].GetDouble();
	data->color =
		PERColor(
			m_document[id]["COLOR_R"].GetInt(),
			m_document[id]["COLOR_G"].GetInt(),
			m_document[id]["COLOR_B"].GetInt()
		);
	data->borderOn = m_document[id]["BORDER_ON"].GetInt();
	data->borderWidth = m_document[id]["BORDER_WIDTH"].GetInt();
	data->borderColor =
		PERColor(
			m_document[id]["BORDER_COLOR_R"].GetInt(),
			m_document[id]["BORDER_COLOR_G"].GetInt(),
			m_document[id]["BORDER_COLOR_B"].GetInt()
		);

	// enum 데이터 설정
	std::string shape = m_document[ id ][ "SHAPE" ].GetString();
	if ( strcmp(shape.c_str(), "ellipse") == 0 ) data->shape = PERShapeType::ELLIPSE;
	else if ( strcmp(shape.c_str(), "triangle") == 0 ) data->shape = PERShapeType::TRIANGLE;
	else if ( strcmp(shape.c_str(), "rectangle") == 0 ) data->shape = PERShapeType::RECTANGLE;
	else if ( strcmp(shape.c_str(), "round rectangle") == 0 ) data->shape = PERShapeType::ROUND_RECTANGLE;
	else {
		PERLog::Logger().ErrorWithFormat("Not correct shape: %s", shape);
		data->shape = PERShapeType::RECTANGLE;
	}

	return data;
}
