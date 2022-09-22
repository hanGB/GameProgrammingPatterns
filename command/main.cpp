#include "stdafx.h"
#include "input_handler.h"
#include "child_of_command.h"

int main() 
{
	InputHandler* inputHandler = new InputHandler();
	inputHandler->SetButton(new JumpCommand(), new NullCommand(), new ReloadCommand(), new FireCommand());
	while (true) {
		inputHandler->HandleInput();

		if (GetAsyncKeyState(VK_ESCAPE)) break;
	}

	delete inputHandler;
}