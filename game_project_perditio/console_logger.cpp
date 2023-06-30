#include "stdafx.h"
#include "console_logger.h"

ConsoleLogger::ConsoleLogger()
{
	AllocConsole();
	FILE* stream;
	freopen_s(&stream, "CONIN$", "r", stdin);
	freopen_s(&stream, "CONOUT$", "w", stderr);
	freopen_s(&stream, "CONOUT$", "w", stdout);

	Info("能贾芒 积己");
}

ConsoleLogger::~ConsoleLogger()
{
	Info("能贾芒 力芭");

	FreeConsole();
}

void ConsoleLogger::Update()
{

}

void ConsoleLogger::Info(const char* text)
{
	std::cout << GetNowTime();
	std::cout << "(INFO)" << text << "\n";
	std::cout << "---------------------------------------------------" << "\n";
}

void ConsoleLogger::Warnning(const char* text)
{
	std::cout << GetNowTime();
	std::cout << "(WARN)" << text << "\n";
	std::cout << "---------------------------------------------------" << "\n";
}

void ConsoleLogger::Error(const char* text)
{
	std::cout << GetNowTime();
	std::cout << "(ERRO)" << text << "\n";
	std::cout << "---------------------------------------------------" << "\n";
}


void ConsoleLogger::SaveLogs()
{
}

void ConsoleLogger::SaveWarnnings()
{
}

void ConsoleLogger::SaveErrors()
{
}

void ConsoleLogger::SaveAll()
{
}
