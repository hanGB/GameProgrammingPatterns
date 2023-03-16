#pragma once
#include "stdafx.h"

char* ReadFile(const char* fileName)
{
	// 데이터 파일 열기
	std::fstream in;
	in.open(fileName);

	// 파일 길이 얻기
	in.seekg(0, std::ios::end);
	size_t length = in.tellg();
	in.seekg(0, std::ios::beg);

	char* data = new char[length + 1];

	// 길이 만큼 파일 읽기
	in.read(data, length);
	data[length] = '\0';

	in.close();

	return data;
}