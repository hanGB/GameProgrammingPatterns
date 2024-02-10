#include "stdafx.h"
#include "json_data_reader.h"

JSONDataReader::JSONDataReader( )
{

}

JSONDataReader::~JSONDataReader( )
{
}

void JSONDataReader::ReadJson(const char* fileName)
{
	rapidjson::Document document;

	// 파일 읽기
	char buffer[ 10000 ];

	FILE* file;
	fopen_s(&file, fileName, "rb");
	rapidjson::FileReadStream is(file, buffer, sizeof(buffer));
	fclose(file);

	// json 파싱
	document.ParseStream(is);
	PERLog::Logger( ).Info(document[ "MONSTER_KOPPER" ][ "NAME_ENG" ].GetString( ));
	PERLog::Logger( ).Info(document[ "MONSTER_KOPPER" ][ "NAME_KOR" ].GetString( ));

	document.RemoveAllMembers( );
}
