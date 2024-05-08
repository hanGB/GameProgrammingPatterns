#pragma once

class JSONDataReader {
public:
	JSONDataReader( );
	~JSONDataReader( );

	void ReadMonsterData(const char* fileName, std::map<std::string, MonsterData*>& dataMap);
	void ReadTranslateData(const char* fileName, std::map<std::string, TranslateData*>& dataMap);
	void ReadVisualData(const char* fileName, std::map<std::string, VisualData*>& dataMap);

private:
	const int c_BUFFER_SIZE = 1'000'000;

	void ReadJson(const char* fileName);
	 
	// 한글 깨짐 해결을 위한 변환 함수
	std::wstring StringToWString(const std::string& var);

	MonsterData* MakeMonsterData(const char* id);
	TranslateData* MakeTranslateData(const char* id);
	VisualData* MakeVisualData(const char* id);

	rapidjson::Document m_document;
};