#include "Player.h"
#include "Math/MathUtilis.h"
#include "Projectile.h"
#include "Engine.h"
#include "Enemy.h"
#include "Asteroid.h"
#include "Kamikazi.h"
#include "PowerUp.h"
#include <memory>

Player::Player(const nc::Transform transform, std::shared_ptr<nc::Texture> texture, float speed) : nc::Actor{ transform, texture }, speed{ speed } {}

void Player::Initialize(){
	std::unique_ptr locator = std::make_unique<Actor>();
	locator->transform.localposition = nc::Vector2{ -15, 0 };
	locator->transform.localrotation = nc::DegToRad(0);
	AddChild(std::move(locator));

	locator = std::make_unique<Actor>();
	locator->transform.localposition = nc::Vector2{ 0,5 };
	AddChild(std::move(locator));

	locator = std::make_unique<Actor>();
	locator->transform.localposition = nc::Vector2{ 0,-5 };
	AddChild(std::move(locator));

	lives = 20;
	//std::unique_ptr visualLives = std::make_unique<Actor>(nc::Transform{}, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Lives.txt"));
	//visualLives->transform.localposition = nc::Vector2{ -10,0 };
	//AddChild(std::move(visualLives));
};


void Player::Update(float dt)
{
	Actor::Update(dt);
	
	//movement
	float thrust = 0;
	if (scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_A) == nc::InputSystem::eKeyState::Held) transform.rotation -= 5 * dt;
	if (scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_D) == nc::InputSystem::eKeyState::Held) transform.rotation += 5 * dt;
	if (scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_W) == nc::InputSystem::eKeyState::Held) thrust = speed;

	std::shared_ptr<nc::Texture> bullet = scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("particle01.png", scene->engine->Get<nc::Renderer>());

	//scene->AddActor(std::make_unique <Enemy>(nc::Transform{ {nc::RandomRange(0, 800), nc::RandomRange(0, 600)}, nc::RandomRange(0.0f, nc::TwoPi), 0.25f
	//	}, engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Enemy.png", engine->Get<nc::Renderer>()), 75.0f));

	//acceleration
	nc::Vector2 acceleration;
	acceleration = nc::Vector2::Rotate(nc::Vector2::right, transform.rotation) * thrust;
	////nc::Vector2 gravity = (nc::Vector2{ 400, 300 } - transform.position).Normalized() * 50;
	//nc::Vector2 gravity = (nc::Vector2::down - transform.position).Normalized() * 50;
	//acceleration += gravity;

	velocity += acceleration * dt;
	transform.position += velocity * dt;
	velocity *= .985f;

	transform.position.x = nc::Wrap(transform.position.x, 0.0f, 800.0f);
	transform.position.y = nc::Wrap(transform.position.y, 0.0f, 800.0f);

	//fire
	fireTimer -= dt ;
	immunityTimer -= dt;
	PUTimer -= dt;
	if (PUTimer <= 0) {
		doubleShot = false;
	}
	else {
		doubleShot = true;
	}



	//if (fireTimer <= 0 && scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == nc::InputSystem::eKeyState::Pressed) {
	if (scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == nc::InputSystem::eKeyState::Held) {
		
		fireTimer = fireRate;
		//std::vector<nc::Vector2> points = { {-5,-5}, {5,-5}, {0,8}, {-5,-5} };

		//scene->AddActor(std::make_unique<Player>(nc::Transform(nc::Vector2(400.0f, 300.0f), 0.0f, 0.25f), texture, 300.0f));
		//std::shared_ptr<nc::Texture> texture = std::make_shared<nc::Texture>(points, nc::Texture(""));
		
		if (doubleShot) {
			scene->engine->Get<nc::AudioSystem>()->PlayAudio("PlayerShoot");
			{
				nc::Transform t = children[1]->transform;
				t.scale = 1.0f;

				std::unique_ptr<Projectile> projectile_R = std::make_unique<Projectile>(t, bullet, 600.0f);
				projectile_R->tag = "Player";
				scene->AddActor(std::move(projectile_R));
			}

			nc::Transform t = children[2]->transform;
			t.scale = 1.0f;

			std::unique_ptr<Projectile> projectile_L = std::make_unique<Projectile>(t, bullet, 600.0f);
			projectile_L->tag = "Player";
			scene->AddActor(std::move(projectile_L));
		}
		else{	
			nc::Transform t = children[0]->transform;
			t.scale = 1.0f;
			t.position = children[0]->transform.position;
			
			std::unique_ptr<Projectile> projectile_M = std::make_unique<Projectile>(t, bullet, 600.0f);
			//scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Enemy.png", scene->engine->Get<nc::Renderer>())

			//std::cout << "bang" << &bullet << std::endl;
			projectile_M->tag = "Player";
			scene->AddActor(std::move(projectile_M));

			//tryout code
			/*std::shared_ptr<nc::Texture> particleTexture = scene->engine->Get<nc::ResourceSystem>()
				->Get<nc::Texture>("particle01.png", scene->engine->Get<nc::Renderer>());

			scene->engine->Get<nc::ParticleSystem>()->Create(t.position, 10, 6.0f, particleTexture, 100.0f);*/
		}
	}

	//scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 5, 2, nc::Color::cyan, 100);
	//std::vector<nc::Color> colors = { nc::Color::white, nc::Color::blue, nc::Color::green };
	//scene->engine->Get<nc::ParticleSystem>()->Create(children[0]->transform.position, 3, 2 ,colors, 50, children[0]->transform.rotation, nc::DegToRad(30));

}

void Player::OnCollision(Actor* actor)
{
	if ((dynamic_cast<Projectile*>(actor) && actor->tag == "Enemy") || (dynamic_cast<Enemy*>(actor)) || (dynamic_cast<Asteroid*>(actor)) || (dynamic_cast<Kame*>(actor))) {
		if (immunityTimer <= 0) {
			if (lives == 1) {
				destroy = true;
				actor->destroy = true;
				scene->engine->Get<nc::AudioSystem>()->PlayAudio("explosion");

				nc::Event event;
				event.name = "PlayerDead";
				//event.data = std::string("ahhhhhhhhh, i ded");
				scene->engine->Get<nc::EventSystem>()->Notify(event);
			}
			else
			{
				//actor->destroy = true;
				lives--;
				immunityTimer = immunityRate;

				nc::Event event;
				event.name = "PlayerLoseLife";
				event.data = lives;
				scene->engine->Get<nc::EventSystem>()->Notify(event);
			}
		}
	}
	else if (dynamic_cast<PowerUp*>(actor)) {
		//actor->destroy = true;
		/*doubleShot = true;*/
		PUTimer = PUTimeRate;
	}
}

int Player::GetLives()
{
	return lives;
}

