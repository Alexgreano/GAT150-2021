#include "Projectile.h"
#include "Math/MathUtilis.h"
#include "Engine.h"
#include "Player.h"
#include "Enemy.h"
#include "Asteroid.h"
#include "Kamikazi.h"
#include "PowerUp.h"


void Projectile::Update(float dt)
{
	Actor::Update(dt);

	lifetime -= dt;
	if (lifetime <= 0) destroy = true;

	transform.position += nc::Vector2::Rotate(nc::Vector2::right, transform.rotation) * speed * dt;
	transform.position.x = nc::Wrap(transform.position.x, 0.0f, 800.0f);
	transform.position.y = nc::Wrap(transform.position.y, 0.0f, 600.0f);

	/*std::vector<nc::texture> textures = { nc::texture::white, nc::texture::red, nc::texture::green, nc::texture::blue, nc::texture::orange, nc::texture::purple, nc::texture::cyan, nc::texture::yelllow };
	scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 5, .5f, textures[nc::RandomRange(0, texture.size())], 50);*/
}

void Projectile::OnCollision(Actor* actor)
{
	//if (!dynamic_cast<Asteroid*>(actor)) {
		if (actor ->tag != actor->tag)
		destroy = true;
		
		/*scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 200, 1, nc::texture::red, 100);
		scene->engine->Get<nc::AudioSystem>()->PlayAudio("explosion");*/
	//}
}
