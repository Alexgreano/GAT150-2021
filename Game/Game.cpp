#include "Game.h"
#include "GameComponent/PlayerComponent.h"
#include "GameComponent/EnemyComponent.h"
#include "GameComponent/PickUpComponent.h"
#include "GameComponent/ExtraLifeComponent.h"
#include "GameComponent/TargetComponent.h"
#include "GameComponent/ProjectileComponent.h"

//int global = 10;


void Game::Initialize()
{
	//create engine
	engine = std::make_unique<nc::Engine>();
	engine->Startup();
	engine->Get<nc::Renderer>()->Create("GAT150", 768, 700);

	//register classes
	REGISTER_CLASS(PlayerComponent);
	REGISTER_CLASS(EnemyComponent);
	REGISTER_CLASS(PickUpComponent);
	REGISTER_CLASS(ExtraLifeComponent);
	REGISTER_CLASS(TargetComponent);
	REGISTER_CLASS(ProjectileComponent);

	//create scene
	scene = std::make_unique<nc::Scene>();
	scene->engine = engine.get();

	nc::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	nc::SetFilePath("../Resources");

	engine->Get<nc::EventSystem>()->Subscribe("add_score", std::bind(&Game::OnAddScore, this, std::placeholders::_1));
 	engine->Get<nc::EventSystem>()->Subscribe("take_life", std::bind(&Game::OnTakeLives, this, std::placeholders::_1));

	

}

void Game::Shutdown()
{
}

void Game::Update(){
	engine->Update();
	

	if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eKeyState::Pressed) {
		quit = true;
	}

	switch (state)
	{
	case Game::eState::Reset:
		Reset();
		break;
	case Game::eState::Title:
		Title();
		break;
	case Game::eState::StartGame:
		StartGame();
		break;
	case Game::eState::StartLevel:
		StartLevel();
		break;
	case Game::eState::Level:
		Level();
		break;
	case Game::eState::PlayerDead:
		PlayerDead();
		break;
	case Game::eState::GameOver:
		GameOver();
		break;
	default:
		break;
	}

	//updateScore
	auto scoreActor = scene->FindActor("score");
	if (scoreActor) {
		scoreActor->GetComponent<nc::TextComponent>()->SetText(std::to_string(score));

	}

	//updateLives
	auto livesActor = scene->FindActor("lives");
	if (livesActor) {
		livesActor->GetComponent<nc::TextComponent>()->SetText(std::to_string(lives));

	}

	if (lives <= 0) {
		state = eState::GameOver;
	}

	scene->Update(engine->time.deltaTime);
}

void Game::Draw(){
	engine->Get<nc::Renderer>()->BegineFrame();

	scene->Draw(engine->Get<nc::Renderer>());
	engine->Draw(engine->Get<nc::Renderer>());
	
	engine->Get<nc::Renderer>()->EndFrame();
}

void Game::Reset()
{
	scene->RemoveAllActors();

	rapidjson::Document document;
	bool success = nc::json::Load("title.txt", document);
	assert(success);

	scene->Read(document);

	state = eState::Title;
}

void Game::Title()
{
	if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == nc::InputSystem::eKeyState::Pressed) {
		auto title = scene->FindActor("Title");
		assert(title);
		title->active = false;

		state = eState::StartGame;
	}
}

void Game::StartGame()
{
	rapidjson::Document document;
	bool success = nc::json::Load("scene.txt", document);
	assert(success);

	scene->Read(document);
	nc::TileMap tilemap;
	tilemap.scene = scene.get();
	success = nc::json::Load("map.txt", document);
	assert(success);
	tilemap.Read(document);
	tilemap.Create();

	state = eState::StartLevel;
	stateTimer = 0;

}

void Game::StartLevel()
{
	stateTimer += engine->time.deltaTime;
	if (stateTimer >= 1) {
		auto player = nc::ObjectFactory::Instance().Create<nc::Actor>("Player");
		player->transform.position = nc::Vector2{ 100, 600 };
		scene->AddActor(std::move(player));

		coinSpawnTimer = 2;
		enemySpawnTimer = 2;
		extraLifeSpawnTimer = 2;
		state = eState::Level;
	}
}

void Game::Level()
{
	enemySpawnTimer -= engine->time.deltaTime;
	extraLifeSpawnTimer -= engine->time.deltaTime;
	coinSpawnTimer -= engine->time.deltaTime;
	shootTimer -= engine->time.deltaTime;
	if (enemySpawnTimer <= 0) {
		enemySpawnTimer = 1.5;

		auto enemy = nc::ObjectFactory::Instance().Create<nc::Actor>("Enemy");
		enemy->transform.position = nc::Vector2{ nc::RandomRandInt(100, 600),  nc::RandomRandInt(100, 600) };
		scene->AddActor(std::move(enemy));
	}

	if (extraLifeSpawnTimer <= 0) {
		extraLifeSpawnTimer = nc::RandomRange(30, 40);

		auto extraLife = nc::ObjectFactory::Instance().Create<nc::Actor>("life");
		extraLife->transform.position = nc::Vector2{ nc::RandomRandInt(100, 600),  nc::RandomRandInt(100, 600) };
		scene->AddActor(std::move(extraLife));
	}

	if (coinSpawnTimer <= 0) {
		coinSpawnTimer = nc::RandomRange(5, 10);

		auto coin = nc::ObjectFactory::Instance().Create<nc::Actor>("Coin");
		coin->transform.position = nc::Vector2{ nc::RandomRandInt(100, 600),  nc::RandomRandInt(100, 600) };
		scene->AddActor(std::move(coin));
	}

	if ((engine->Get<nc::InputSystem>()->GetButtonState((int)nc::InputSystem::eMouseButton::Left) == nc::InputSystem::eKeyState::Held) && shootTimer <= 0) {
		shootTimer = 0.5f;
		
		auto projectile = nc::ObjectFactory::Instance().Create<nc::Actor>("Projectile");
		projectile->transform.position = nc::Vector2{ scene->FindActor("Player")->transform.position };
		scene->AddActor(std::move(projectile));

		std::cout << "projectile create" << std::endl;
	}
}

void Game::PlayerDead()
{
}

void Game::GameOver()
{
	scene->RemoveAllActors();
}

void Game::OnAddScore(const nc::Event& event)
{
	score += std::get<int>(event.data);
}

void Game::OnTakeLives(const nc::Event& event)
{
	lives -= std::get<int>(event.data);
}
