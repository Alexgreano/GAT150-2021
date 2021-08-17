#pragma once
#include "Engine.h"
#include <Actor/Player.h>
#include <Actor/Enemy.h>
#include <Actor/Asteroid.h>
#include <Actor/Kamikazi.h>
#include <Actor/PowerUp.h>


class Game {
public:
	enum class eState {
		Title,
		Startgame,
		StartLevel,
		Game,
		GameOver,
		Restart
	};

public: 
	void Initialize();
	void Shutdown();

	void Update();
	void Draw();

	bool IsQuit() { return quit; };

private:
	void UpdateTitle(float dt);
	void UpdateStartLevel(float dt, size_t level);
	void OnAddPoints(const nc::Event& event);
	void OnPlayerDead(const nc::Event& event);
	void PlayerLoseLife(const nc::Event& event);

public:
	std::unique_ptr<nc::Engine> engine;
	std::unique_ptr<nc::Scene> scene;

private:
	bool quit{ false };
	eState state = eState::Title;
	float stateTimer = 0.0f;
	float SpawnTimer = 0.0f;

	std::shared_ptr<nc::Texture> scoreT;
	std::shared_ptr<nc::Texture> livesT;


	nc::Transform t;
	nc::Transform scoreP;
	nc::Transform livesP;

	size_t score = 0;
	size_t lives = 0;
	size_t level = 0;
	nc::AudioChannel musicChannel;
	std::shared_ptr<nc::Texture> particleTexture;
	std::shared_ptr<nc::Texture> textTexture;
	std::shared_ptr<nc::Texture> gameOver;
	//std::shared_ptr<nc::Texture> playerTexture;
};