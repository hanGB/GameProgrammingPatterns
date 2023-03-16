#include "stdafx.h"
#include "interpreter.h"

int main()
{
	Interpreter* interpreter = new Interpreter();
	
	if (!interpreter->ExcuteFile("data.txt")) {
		std::cout << "ERROR!";
	}

	delete interpreter;
}
