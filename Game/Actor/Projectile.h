#pragma once
#include "Object/Actor.h"

class Projectile : public nc::Actor {
public:
	Projectile(const nc::Transform transform, std::shared_ptr<nc::Texture> texture, float speed) : nc::Actor{ transform, texture }, speed{ speed } {};
	virtual void Update(float dt) override;

	void OnCollision(Actor* actor) override;


private:
	float lifetime{ 1.5f  };
	float speed{ 300.0f };
};