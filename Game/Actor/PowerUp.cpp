#include "PowerUp.h"
#include "Projectile.h"
#include "Math/MathUtilis.h"
#include "Engine.h"
#include "Player.h"


void PowerUp::Update(float dt)
{
	Actor::Update(dt);

	if (scene->GetActor<Player>()) {
		nc::Vector2 direction = scene->GetActor<Player>()->transform.position - transform.position;
		nc::Vector2 forward = nc::Vector2::Rotate(nc::Vector2::right, transform.rotation);
		
	}

	transform.position += nc::Vector2::Rotate(nc::Vector2::right, transform.rotation) * speed * dt;
	transform.position.x = nc::Wrap(transform.position.x, 0.0f, 800.0f);
	transform.position.y = nc::	Wrap(transform.position.y, 00.f, 800.0f);
}

void PowerUp::OnCollision(Actor* actor) {
	if (dynamic_cast<Player*>(actor)) {

		destroy = true;

		//scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 200, 1, nc::Color::red, 100);
		std::shared_ptr<nc::Texture> particleTexture = scene->engine->Get<nc::ResourceSystem>()
			->Get<nc::Texture>("particle01.png", scene->engine->Get<nc::Renderer>());
		//scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 10, 6.0f, particleTexture, 100.0f);
		scene->engine->Get<nc::AudioSystem>()->PlayAudio("powerUp",2.0f,1.0f,false);
	}
	
}

