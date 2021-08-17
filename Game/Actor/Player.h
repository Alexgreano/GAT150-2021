#pragma once
#include"Object/Actor.h"

class Player : public nc::Actor {
public:
	Player(const nc::Transform transform, std::shared_ptr<nc::Texture> texture, float speed);

	void Initialize() override;

	virtual void Update(float dt) override;
	void OnCollision(Actor* actor) override;
	int GetLives();

private:
	int lives{ 0 };
	float fireTimer{ 0 };
	float fireRate = { 0.2f };
	float speed{ 0 };
	nc::Vector2 velocity;
	float immunityTimer{ 0 };
	float immunityRate = { 0.5f };
	float PUTimer{ 0 };
	float PUTimeRate = { 6.0f };
	bool doubleShot = false;
};