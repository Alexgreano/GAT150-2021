#include "Engine.h"
#include <iostream>
#include <SDL.h>
#include <SDL_Image.h>


int main(int, char**)
{

	nc::Engine engine;
	engine.Startup();

	engine.Get<nc::Renderer>()->Create("GAT150", 800, 600);

	nc::Scene scene;
	scene.engine = &engine;

	//SDL_Point screen{ 800,600 };

	nc::SetFilePath("../Resources");

	std::shared_ptr<nc::Texture> texture = engine.Get<nc::ResourceSystem>()
		->Get<nc::Texture>("sf2.png", engine.Get<nc::Renderer>());

	for (size_t i = 0; i < 50; i++) {
		nc::Transform transform{ {nc::RandomRange(0,800), nc::RandomRange(0,600) }, nc::RandomRange(0,360), 1.0f };
		std::unique_ptr<nc::Actor> actor = std::make_unique<nc::Actor>(transform, texture);
		scene.AddActor(std::move(actor));

	}

	bool quit = false;
	SDL_Event event;
	while (!quit)
	{
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}

		engine.Update(0);
		quit = (engine.Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) ==
			nc::InputSystem::eKeyState::Pressed);
		scene.Update(0);

		engine.Get<nc::Renderer>()->BegineFrame();

		//scene.Draw()
		scene.Draw(engine.Get<nc::Renderer>());

		//nc::Vector2 position{ 300,400 };

		engine.Get<nc::Renderer>()->EndFrame();
	}
	
	SDL_Quit();

	return 0;
}
