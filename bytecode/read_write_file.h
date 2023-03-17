#pragma once
#include "stdafx.h"

char* ReadFile(const char* fileName)
{
	// 데이터 파일 열기
	std::ifstream in;
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

char* ReadFileInBinary(const char* fileName, int& size)
{
	// 데이터 파일 열기
	std::ifstream in;
	in.open(fileName, std::ios::binary);

	// 파일 길이 얻기
	in.seekg(0, std::ios::end);
	size = (int)in.tellg();
	in.seekg(0, std::ios::beg);

	char* data = new char[size];

	// 길이 만큼 파일 읽기
	in.read(data, size);

	in.close();

	return data;
}

void WriteFileInBinary(const char* fileName, char* data, int size)
{
	// 데이터 파일 열기
	std::ofstream out;
	out.open(fileName, std::ios::out | std::ios::binary);

	// 파일에 쓰기
	out.write(data, size);

	out.close();
}
