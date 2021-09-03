#include "EnemyComponent.h"
#include "Engine.h"
#include <cstdlib>

using namespace nc;


void EnemyComponent::Update()
{
	Actor* player = owner->scene->FindActor("Player");
	if (player) {
		Vector2 direction = player->transform.position - owner->transform.position;

		if (abs(direction.x) < 200 && abs(direction.y) < 200) {
			Vector2 force = direction.Normalized() * speed;
	
			PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>();
			assert(physicsComponent);

			physicsComponent->ApplyForce(force);

		}

	}
}

bool EnemyComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool EnemyComponent::Read(const rapidjson::Value& value)
{
	JSON_READ(value, speed);
	return true;
}