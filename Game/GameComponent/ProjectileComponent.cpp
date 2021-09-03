#include "ProjectileComponent.h"

using namespace nc;
ProjectileComponent::~ProjectileComponent()
{

	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_enter", owner);
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_exit", owner);
}

void ProjectileComponent::Create()
{
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter", std::bind(&ProjectileComponent::OnCollisionEnter, this, std::placeholders::_1), owner);
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_exit", std::bind(&ProjectileComponent::OnCollisionExit, this, std::placeholders::_1), owner);

	owner->scene->engine->Get<AudioSystem>()->AddAudio("shoot", "audio/shoot.wav");

	fire2 = owner->scene->engine->Get<InputSystem>()->GetMousePosition();
}

void ProjectileComponent::Update()
{
	/*Vector2 force = Vector2::zero;

	PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>();
	assert(physicsComponent);

	physicsComponent->ApplyForce(force);*/
	lifetime -= owner->scene->engine->time.deltaTime;
	if (lifetime <= 0)
	{
		owner->destroy = true;
	}

	if (!init)
	{
		init = true;

		Vector2 direction = fire2 - owner->transform.position;
		Vector2 force = direction.Normalized() * 20;

		PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>();
		assert(physicsComponent);
		physicsComponent->ApplyForce(force);
	}
}

void ProjectileComponent::OnCollisionEnter(const nc::Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "Enemy")) {
		//owner->scene->engine->Get<AudioSystem>()->PlayAudio("coin");
		owner->destroy = true;
		actor->destroy = true;

		Event event;
		event.name = "add_score";
		event.data = 20;

		owner->scene->engine->Get<EventSystem>()->Notify(event);
	}
}

void ProjectileComponent::OnCollisionExit(const nc::Event& event)
{
}

bool ProjectileComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool ProjectileComponent::Read(const rapidjson::Value& value)
{
	//JSON_READ(value, speed);
	return true;
}
