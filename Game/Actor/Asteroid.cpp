#include "Asteroid.h"
#include "Projectile.h"
#include "Math/MathUtilis.h"
#include "Engine.h"
#include "Player.h"
#include "Enemy.h"


void Asteroid::Update(float dt)
{
	Actor::Update(dt);

	if (scene->GetActor<Player>()) {
		nc::Vector2 direction = scene->GetActor<Player>()->transform.position - transform.position;
		nc::Vector2 forward = nc::Vector2::Rotate(nc::Vector2::right, transform.rotation);
	}

	transform.position += nc::Vector2::Rotate(nc::Vector2::right, transform.rotation) * speed * dt;
	transform.position.x = nc::Wrap(transform.position.x, 0.0f, 800.0f);
	transform.position.y = nc::Wrap(transform.position.y, 00.f, 800.0f);
}

void Asteroid::OnCollision(Actor* actor) {


  	if ((dynamic_cast<Projectile*>(actor)) && ((actor->tag == "Player"))) {
		destroy = true;
		nc::Event event;
		event.name = "AddPoints";
		event.data = 100;
		scene->engine->Get<nc::EventSystem>()->Notify(event);
		//scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 200, 1, nc::Color::red, 100);
		scene->engine->Get<nc::AudioSystem>()->PlayAudio("explosion");
	}

	if (dynamic_cast<Player*>(actor)) {
		destroy = true;
		//actor->destroy = true;

		//scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 200, 1, nc::Color::red, 100);
		scene->engine->Get<nc::AudioSystem>()->PlayAudio("explosion");
	}
}

