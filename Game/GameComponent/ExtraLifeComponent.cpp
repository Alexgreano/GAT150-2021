#include "ExtraLifeComponent.h"

using namespace nc;
ExtraLifeComponent::~ExtraLifeComponent()
{
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_enter", owner);
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_exit", owner);
}

void ExtraLifeComponent::Create()
{
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter", std::bind(&ExtraLifeComponent::OnCollisionEnter, this, std::placeholders::_1), owner);
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_exit", std::bind(&ExtraLifeComponent::OnCollisionExit, this, std::placeholders::_1), owner);

	owner->scene->engine->Get<AudioSystem>()->AddAudio("gainLife", "audio/gainLife.wav");
	owner->scene->engine->Get<AudioSystem>()->AddAudio("lava", "audio/lava.wav");
}

void ExtraLifeComponent::Update()
{
	//coin = owner->scene->FindActor("Coin");
	Vector2 force = Vector2::zero;

	PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>();
	assert(physicsComponent);

	physicsComponent->ApplyForce(force);
}

void ExtraLifeComponent::OnCollisionEnter(const nc::Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "Player")) {
		owner->scene->engine->Get<AudioSystem>()->PlayAudio("gainLife");
		owner->destroy = true;

		Event event;
		event.name = "take_life";
		event.data = -1;

		owner->scene->engine->Get<EventSystem>()->Notify(event);
	}
	if (istring_compare(actor->tag, "lava")) {
		owner->scene->engine->Get<AudioSystem>()->PlayAudio("lava");
		owner->destroy = true;
	}

	std::cout << actor->tag << std::endl;
}

void ExtraLifeComponent::OnCollisionExit(const nc::Event& event)
{
}

bool ExtraLifeComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool ExtraLifeComponent::Read(const rapidjson::Value& value)
{
	return true;
}
