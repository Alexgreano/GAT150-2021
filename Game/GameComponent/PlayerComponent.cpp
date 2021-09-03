#include "PlayerComponent.h"
#include "Engine.h"
using namespace nc;


PlayerComponent::~PlayerComponent()
{
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_enter", owner);
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_exit", owner);
}

void PlayerComponent::Create()
{
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter", std::bind(&PlayerComponent::OnCollisionEnter, this, std::placeholders::_1), owner);
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_exit", std::bind(&PlayerComponent::OnCollisionExit, this, std::placeholders::_1), owner);

	owner->scene->engine->Get<AudioSystem>()->AddAudio("hurt", "audio/hurt.wav");
	owner->scene->engine->Get<AudioSystem>()->AddAudio("lava", "audio/lava.wav");

	spriteAnimationComponent = owner->GetComponent<SpriteAnimationComponent>();
}

void PlayerComponent::Update()
{
	force = Vector2::zero;
	if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_A) == InputSystem::eKeyState::Held) {
		force.x -= speed;
	}
	if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_D) == InputSystem::eKeyState::Held) {
		force.x += speed;
	}

	if (jump == true > 0 && owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == InputSystem::eKeyState::Pressed) {
		force.y -= jumpAmount;
		jump = false;
	}
	
	/*if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_W) == InputSystem::eKeyState::Held) {
		force.y -= speed;
	}
	if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_S) == InputSystem::eKeyState::Held) {
		force.y += speed;
	}*/

	PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>();
	assert(physicsComponent);

	physicsComponent->ApplyForce(force);

	spriteAnimationComponent = owner->GetComponent<SpriteAnimationComponent>();
	assert(spriteAnimationComponent);
	if (physicsComponent->velocity.x > 0) spriteAnimationComponent->StartSequence("walk_right");
	else if (physicsComponent->velocity.x < 0)spriteAnimationComponent->StartSequence("walk_left");
	else spriteAnimationComponent->StartSequence("idle");

	immunityTimer -= owner->scene->engine->time.deltaTime;
	//std::cout << immunityTimer << std::endl;
}

void PlayerComponent::OnCollisionEnter(const Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "ground")) {
		contacts.push_back(actor);
		jump = true;
	}

	if (istring_compare(actor->tag, "enemy") || istring_compare(actor->tag, "lava")) {
		if (immunityTimer <= 0) {
			owner->scene->engine->Get<AudioSystem>()->PlayAudio("hurt");


			//bounce on lava 
			if (istring_compare(actor->tag, "lava")) {
				force.y -= (jumpAmount / 2);
				owner->scene->engine->Get<AudioSystem>()->PlayAudio("lava");
			}
			spriteAnimationComponent->StartSequence("hurt");

			Event event;
			event.name = "take_life";
			event.data = 1;

			owner->scene->engine->Get<EventSystem>()->Notify(event);
			immunityTimer = 2;
		}
	}

	std::cout << actor->tag << std::endl;
}

void PlayerComponent::OnCollisionExit(const Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "ground")) {
		contacts.remove(actor);
	}

	std::cout << actor->tag << std::endl;
}



bool PlayerComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool PlayerComponent::Read(const rapidjson::Value& value)
{
	JSON_READ(value, speed);
	JSON_READ(value, jumpAmount);
	return true;
}



