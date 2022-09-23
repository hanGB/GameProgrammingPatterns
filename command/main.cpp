#include "stdafx.h"
#include "input_handler.h"
#include "child_of_command.h"
#include "game_actor.h"
#include "ai_controller.h"

int main() 
{
	InputHandler* inputHandler = new InputHandler();
	inputHandler->SetButton(new JumpCommand(), new NullCommand(), new ReloadCommand(), new FireCommand());
	GameActor* player = new GameActor("Player");
	
	AiController* aiController = new AiController();
	GameActor* monster = new GameActor("Monster");

	while (true) {
		Command* input = inputHandler->HandleInput();
		if (input) {
			input->Execute(*player);
		}

		Command* givenCommand = aiController->GiveCommand();
		if (givenCommand) {
			givenCommand->Execute(*monster);
			delete givenCommand;
		}


		if (GetAsyncKeyState(VK_ESCAPE)) break;
	}

	delete inputHandler;
	delete player;
	delete aiController;
	delete monster;
}