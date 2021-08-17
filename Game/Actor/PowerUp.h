#pragma once
#include"Object/Actor.h"

class PowerUp : public nc::Actor {
public:
	PowerUp(const nc::Transform transform, std::shared_ptr<nc::Texture> texture, float speed) : nc::Actor{ transform, texture }, speed{ speed } {tag = "PowerUp"; };

	virtual void Update(float dt) override;
	void OnCollision(Actor* actor) override;



private:
	float speed{ 100 };

};