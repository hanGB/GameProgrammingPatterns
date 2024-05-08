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
	 
	// UTF-8 텍스트를 wstring에 넣음
	void UTF8TextToWString(const char* utf8Text, std::wstring& result);

	MonsterData* MakeMonsterData(const char* id);
	TranslateData* MakeTranslateData(const char* id);
	VisualData* MakeVisualData(const char* id);

	rapidjson::Document m_document;
};