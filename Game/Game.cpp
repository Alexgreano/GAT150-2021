#include "Game.h"
#include "GameComponent/PlayerComponent.h"
#include "GameComponent/EnemyComponent.h"

//int global = 10;


void Game::Initialize()
{
	//create engine
	engine = std::make_unique<nc::Engine>();
	engine->Startup();
	engine->Get<nc::Renderer>()->Create("GAT150", 800, 600);

	//register classes
	REGISTER_CLASS(PlayerComponent);
	REGISTER_CLASS(EnemyComponent);

	//create scene
	scene = std::make_unique<nc::Scene>();
	scene->engine = engine.get();

	nc::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	nc::SetFilePath("../Resources");

	rapidjson::Document document;
	bool success = nc::json::Load("scene.txt", document);
	assert(success);

	scene->Read(document);
	
	////actors
	//std::unique_ptr<nc::Actor> actor = std::make_unique <nc::Actor>(nc::Transform{ nc::Vector2{400, 300}, 0, 1 });
	//{
	//	auto component = nc::ObjectFactory::Instance().Create<nc::SpriteCompnent>("SpriteCompnent");
	//	component->texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("sparkle.png", engine->Get<nc::Renderer>());
	//	actor->AddComponent(std::move(component));
	//	//nc::SpriteAnimationComponent* component = actor->AddComponent<nc::SpriteCompnent>();

	//}
	//scene->AddActor(std::move(actor));
}

void Game::Shutdown()
{
}

void Game::Update(){
	engine->Update();
	

	if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eKeyState::Pressed) {
		quit = true;
	}

	scene->Update(engine->time.deltaTime);
}

void Game::Draw(){
	engine->Get<nc::Renderer>()->BegineFrame();

	scene->Draw(engine->Get<nc::Renderer>());
	engine->Draw(engine->Get<nc::Renderer>());
	
	engine->Get<nc::Renderer>()->EndFrame();
}