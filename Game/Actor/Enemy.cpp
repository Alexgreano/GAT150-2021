#include "Enemy.h"
#include "Projectile.h"
#include "Math/MathUtilis.h"
#include "Engine.h"
#include "Player.h"


void Enemy::Update(float dt)
{
	Actor::Update(dt);

	if (scene->GetActor<Player>()) {
		nc::Vector2 direction = scene->GetActor<Player>()->transform.position - transform.position;
		nc::Vector2 forward = nc::Vector2::Rotate(nc::Vector2::right, transform.rotation);
		
		float turnAngle = nc::Vector2::SignedAngle(forward, direction.Normalized());
		transform.rotation = transform.rotation + turnAngle * (3*dt);

		float angle = nc::Vector2::Angle(forward, direction.Normalized());

		fireTimer -= dt;
		if (fireTimer <= 0 && angle <= nc::DegToRad(10))
		{
			fireTimer = fireRate;
			std::vector<nc::Vector2> points = { {-5,-5}, {5,-5}, {0,8}, {-5,-5} };
			//std::shared_ptr<nc::Texture> texture = std::make_shared<nc::Shape>(points, nc::Color{ 1,1,0 });

			nc::Transform t = transform;
			t.scale = 0.25f;
			std::shared_ptr<nc::Texture> projectile = scene->engine->Get<nc::ResourceSystem>()
				->Get<nc::Texture>("particle01.png", scene->engine->Get<nc::Renderer>());
			
			//projectile->tag = "Enemy";
			scene->AddActor(std::move(std::make_unique <Projectile>(t, projectile, 600.0f)));
			scene->engine->Get<nc::AudioSystem>()->PlayAudio("EnemyShoot");

		}


	}
	
	transform.position += nc::Vector2::Rotate(nc::Vector2::right, transform.rotation) * speed * dt;
	transform.position.x = nc::Clamp(transform.position.x, 0.0f, 800.0f);
	transform.position.y = nc::Clamp(transform.position.y, 00.f, 800.0f);
}

void Enemy::OnCollision(Actor* actor){
	
	if ((dynamic_cast<Projectile*>(actor)) && ((actor->tag == "Player"))) {
		destroy = true;
		nc::Event event;
		event.name = "AddPoints";
		event.data = 300;
		scene->engine->Get<nc::EventSystem>()->Notify(event);
		//scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 200, 1, nc::Color::red, 100);
		scene->engine->Get<nc::AudioSystem>()->PlayAudio("explosion");
	}

	if (dynamic_cast<Player*>(actor)) {
		destroy = true;
		//actor->destroy = true;

		/*std::shared_ptr<nc::Texture> particleTexture = scene->engine->Get<nc::ResourceSystem>()
			->Get<nc::Texture>("particle01.png", scene->engine->Get<nc::Renderer>());
		scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 10, 6.0f, particleTexture, 100.0f); */
		scene->engine->Get<nc::AudioSystem>()->PlayAudio("explosion",1.0f, nc::RandomRange(0.2f, 2.0f),false);
	}
}

