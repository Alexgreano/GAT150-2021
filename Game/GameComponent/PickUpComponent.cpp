#include "PickUpComponent.h"

using namespace nc;
PickUpComponent::~PickUpComponent()
{
		owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_enter", owner);
		owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_exit", owner);
}

void PickUpComponent::Create()
{
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter", std::bind(&PickUpComponent::OnCollisionEnter, this, std::placeholders::_1), owner);
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_exit", std::bind(&PickUpComponent::OnCollisionExit, this, std::placeholders::_1), owner);

	owner->scene->engine->Get<AudioSystem>()->AddAudio("coin", "audio/coin.wav");
}

void PickUpComponent::Update()
{
	//coin = owner->scene->FindActor("Coin");
	Vector2 force = Vector2::zero;

	PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>();
	assert(physicsComponent);

	physicsComponent->ApplyForce(force);
}

void PickUpComponent::OnCollisionEnter(const nc::Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "Player")) {
		owner->scene->engine->Get<AudioSystem>()->PlayAudio("coin");
		owner->destroy = true;
	}

	std::cout << actor->tag << std::endl;
}

void PickUpComponent::OnCollisionExit(const nc::Event& event)
{
}

bool PickUpComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool PickUpComponent::Read(const rapidjson::Value& value)
{
	return true;
}
