#pragma once

#include "Object/Actor.h"

class Asteroid : public nc::Actor {
public:
	Asteroid(const nc::Transform transform, std::shared_ptr<nc::Texture> texture, float speed) : nc::Actor{ transform, texture }, speed{ speed } {tag = "Enemy"; };

	virtual void Update(float dt) override;
	void OnCollision(Actor* actor) override;



private:
	float speed{ 50 };
	float fireTimer{ 0 };
	float fireRate{ 0 };

};