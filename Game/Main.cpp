//#include "Engine.h"
//#include <iostream>
//#include <SDL.h>
//#include <SDL_Image.h>
//#include <Graphics/Font.h>
//#include <cassert>
#include "Game.h"
#include <Graphics/Font.h>

//#define MSG(message) std::cout << #message << std::endl;

int main(int, char**)
{
	//int i = 6;
	//assert(i == 6);

	////std::cout << __func__ << std::endl;
	//MSG("alayna is dumb");

	Game game;
	game.Initialize();

	bool quit = false;
	SDL_Event event;


	while (!quit && !game.IsQuit())
	{
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}
		game.Update();
		game.Draw();
		
	}
	
	SDL_Quit();

	return 0;
}