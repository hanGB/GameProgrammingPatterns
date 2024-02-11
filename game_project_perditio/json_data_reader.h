#pragma once

class JSONDataReader {
public:
	JSONDataReader( );
	~JSONDataReader( );

	void ReadMonsterData(const char* fileName, std::map<std::string, MonsterData*>& dataMap);
	void ReadTranslateData(const char* fileName, std::map<std::string, TranslateData*>& dataMap);

private:
	const int c_BUFFER_SIZE = 1'000'000;

	void ReadJson(const char* fileName);

	MonsterData* MakeMonsterData(const char* id);
	TranslateData* MakeTranslateData(const char* id);

	rapidjson::Document m_document;
};