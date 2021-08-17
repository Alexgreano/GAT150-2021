#include "Game.h"
#include <Graphics/Font.h>

void Game::Initialize()
{
	//create engine
	engine = std::make_unique<nc::Engine>();
	engine->Startup();
	engine->Get<nc::Renderer>()->Create("GAT150", 800, 600);

	//create scene
	scene = std::make_unique<nc::Scene>();
	scene->engine = engine.get();
	nc::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	nc::SetFilePath("../Resources");



	engine->Get<nc::AudioSystem>()->AddAudio("PlayerShoot", "Laser_Shoot1.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("EnemyShoot", "Laser_ShootEnemy.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("Level1", "Level1.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("Level2", "Level2.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("Level3", "Level3.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("explosion", "audio/Explosion.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("powerUp", "audio/Powerup.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("music", "audio/Menu.wav");
	musicChannel = engine->Get<nc::AudioSystem>()->PlayAudio("music", 1, 1, true);

	engine->Get<nc::EventSystem>()->Subscribe("AddPoints", std::bind(&Game::OnAddPoints, this, std::placeholders::_1));
	engine->Get<nc::EventSystem>()->Subscribe("PlayerDead", std::bind(&Game::OnPlayerDead, this, std::placeholders::_1));
	engine->Get<nc::EventSystem>()->Subscribe("PlayerLoseLife", std::bind(&Game::PlayerLoseLife, this, std::placeholders::_1));

	//std::shared_ptr<nc::Texture> texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("sf2.png", engine->Get<nc::Renderer>());
	//particleTexture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("particle01.png", engine->Get<nc::Renderer>());

	t.position = { 400, 200 };
	scoreP.position = { 50, 30 };
	livesP.position = { 400, 30 };

	

	// get font from resource system
	int size = 100;
	std::shared_ptr<nc::Font> font = engine->Get<nc::ResourceSystem>()->Get<nc::Font>("fonts/VTKSDURA3d.ttf", &size);
	// create font texture
	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	// set font texture with font surface
	// add font texture to resource system
	textTexture->Create(font->CreateSurface("___MACHIA___", nc::Color{ 1,0,1 }));
	engine->Get<nc::ResourceSystem>()->Add("textTexture", textTexture);


	gameOver = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	// set font texture with font surface
	// add font texture to resource system
	gameOver->Create(font->CreateSurface("GAMEOVER", nc::Color{ 1,0,1 }));
	engine->Get<nc::ResourceSystem>()->Add("gameOver", gameOver);

	//stateFunction = &Game::UpdateTitle;
}

void Game::Shutdown()
{
}

void Game::Update(){
	engine->Update();
	float dt = engine->time.deltaTime;

	stateTimer += dt;
	SpawnTimer += dt;
	

	switch (state)
	{
	case Game::eState::Title:
		if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == nc::InputSystem::eKeyState::Pressed) {
			state = eState::Startgame;
		}
		break;
	case Game::eState::Startgame:
		score = 0;
		lives = 20;
		state = eState::StartLevel;
		
		break;
	case Game::eState::StartLevel: {
		//scene->engine->Get<nc::AudioSystem>()->PlayAudio("Level1");
		UpdateStartLevel(dt, level);
		
		state = eState::Game;
	}

		break;
	case Game::eState::Game:
		if ((scene->GetActors<Enemy>().size() == 0) && (scene->GetActors<Kame>().size() == 0)) {
			//if ((scene->GetActors<Enemy>().size() == 0)) {

			for (size_t i = 0; i < (nc::RandomRandInt(1, 5)); i++) {
				scene->AddActor(std::make_unique <Enemy>(nc::Transform{ {nc::RandomRange(0, 800), nc::RandomRange(0, 600)}, nc::RandomRange(0.0f, nc::TwoPi), 0.25f
					}, engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Enemy.png", engine->Get<nc::Renderer>()), 75.0f));
			}

			for (size_t i = 0; i < (nc::RandomRandInt(1, 3)); i++) {
				//for (size_t i = 0; i < 1; i++) {

				scene->AddActor(std::make_unique <Asteroid>(nc::Transform{ {nc::RandomRange(0, 800), nc::RandomRange(0, 600)}, nc::RandomRange(0.0f, nc::TwoPi), 0.08f
					}, engine->Get<nc::ResourceSystem>()->Get<nc::Texture>(("asteroid_" + std::to_string(nc::RandomRandInt(1, 5)) + ".png"), engine->Get<nc::Renderer>()), 175.0f));
			}
			for (size_t i = 0; i < (nc::RandomRandInt(1, 5)); i++) {
			//for (size_t i = 0; i < 1; i++) {
					scene->AddActor(std::make_unique <Kame>(nc::Transform{ {nc::RandomRange(0, 800), nc::RandomRange(0, 600)}, nc::RandomRange(0.0f, nc::TwoPi), 0.5f
						}, engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Kame.png", engine->Get<nc::Renderer>()), 75.0f));
			}
			if (nc::RandomRandInt(1, 10) <= 3.0f) {
				scene->AddActor(std::make_unique <PowerUp>(nc::Transform{ {nc::RandomRange(0, 800), nc::RandomRange(0, 600)}, nc::RandomRange(0.0f, nc::TwoPi), 0.03f
					}, engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("powerup.png", engine->Get<nc::Renderer>()), 75.0f));
			}
		}
		break;
	case Game::eState::GameOver:
		break;
	
	case Game::eState::Restart:
			state = eState::StartLevel;
		break;
	default:
		break;
	
	
	}

	//scene->Update(dt);

	if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eKeyState::Pressed) {
		quit = true;
	}

	//if (engine->Get<nc::InputSystem>()->GetButtonState((int)nc::InputSystem::eMouseButton::Left) == nc::InputSystem::eKeyState::Pressed) {
	//	nc::Vector2 position = engine->Get<nc::InputSystem>()->GetMousePosition();
	//	engine->Get<nc::ParticleSystem>()->Create(position, 10, 6.0f, particleTexture, 100.0f);
	//	engine->Get<nc::AudioSystem>()->PlayAudio("explosion", 1, nc::RandomRange(0.2f, 2.0f));
	//	musicChannel.SetPitch(nc::RandomRange(0.2f, 2.0f));
	//	/*engine->Draw()*/
	//	//create particles
	//	std::cout << position.x << " " << position.y << std::endl;
	//}

	scene->Update(engine->time.deltaTime);
}

void Game::Draw(){
	engine->Get<nc::Renderer>()->BegineFrame();

	int sizeT = 20;
	std::shared_ptr<nc::Font> font2 = engine->Get<nc::ResourceSystem>()->Get<nc::Font>("fonts/VTKSDURA3d_2.ttf", &sizeT);
	scoreT = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	scoreT->Create(font2->CreateSurface(std::to_string(score), nc::Color{ 1, 0, 1 }));
	engine->Get<nc::ResourceSystem>()->Add("scoreT", scoreT);

	livesT = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	livesT->Create(font2->CreateSurface(std::to_string(lives), nc::Color{ 1, 0, 1 }));
	engine->Get<nc::ResourceSystem>()->Add("livesT", livesT);

	switch (state)
	{
	case Game::eState::Title:
		/*graphics.SetColor(nc::Color::purple);
		graphics.DrawString(360, static_cast<int>(300 + std::sin(stateTimer * 6.0f)), "Machia");

		graphics.DrawString(300, 400, "Press Enter To Start");*/
		engine->Get<nc::Renderer>()->Draw(textTexture, t);

		break;
	case Game::eState::Startgame:
		break;
	case Game::eState::StartLevel:

		//engine->Get<nc::Renderer>()->Draw(textTexture, t);
		break;
	case Game::eState::Game:
		

		engine->Get<nc::Renderer>()->Draw(scoreT, scoreP);
		engine->Get<nc::Renderer>()->Draw(livesT, livesP);
		break;
	case Game::eState::GameOver:
		engine->Get<nc::Renderer>()->Draw(gameOver, t);

		/*graphics.SetColor(nc::Color::red);
		graphics.DrawString(360, static_cast<int>(300 + std::sin(stateTimer * 6.0f)), "GAME OVER");*/
		break;
	default:
		break;


	}

	/*graphics.SetColor(nc::Color::cyan);
	graphics.DrawString(30,20,std::to_string(score).c_str());
	graphics.DrawString(320,20,std::to_string(lives).c_str());*/

	/*engine->Get<nc::Renderer>()->Draw(STTexture, scoreP);
	engine->Get<nc::Renderer>()->Draw(STTexture, livesP);*/

	//int STSize = 20;
	//textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	//textTexture->Create(font->CreateSurface("ScoreAndLives", nc::Color{ 1,0,1 })); //std::to_string((score).c_str())
	//engine->Get<nc::ResourceSystem>()->Add("textTexture", textTexture);
	//t.position = { 30, 50 };
	//engine->Get<nc::Renderer>()->Draw(textTexture, t);
	

	//scene.Draw();
	scene->Draw(engine->Get<nc::Renderer>());
	engine->Draw(engine->Get<nc::Renderer>());

	//nc::Vector2 position{ 300,400 };

	engine->Get<nc::Renderer>()->EndFrame();
}

void Game::OnAddPoints(const nc::Event& event)
{
	score += std::get<int>(event.data);
}

void Game::PlayerLoseLife(const nc::Event& event)
{
	lives = std::get<int>(event.data);
}


void Game::OnPlayerDead(const nc::Event& event)
{
	//std::cout << std::get<std::string>(event.data) << std::endl;
	state = eState::GameOver;
	lives = 0;
}

void Game::UpdateStartLevel(float dt, size_t level)
{
	std::shared_ptr<nc::Texture> texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("HeroShip.png", engine->Get<nc::Renderer>());
	/*scene->AddActor(std::make_unique<Player>(nc::Transform(nc::Vector2(400.0f, 300.0f), 0.0f, 0.25f), texture, 300.0f));*/


	std::unique_ptr<Player> player = std::make_unique <Player>(nc::Transform(nc::Vector2(400.0f, 300.0f), 0.0f, 0.25f), texture, 300.0f);
	player->tag = "Player";
	scene->AddActor(std::move(player));

		//engine->Get<nc::ParticleSystem>()->Create(position, 10, 6.0f, particleTexture, 100.0f);

		

}
