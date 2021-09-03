#pragma once
#include "Component/Component.h"
#include "Framework/EventSystem.h"
#include "Engine.h"

class ProjectileComponent : public nc::Component {
public:
	std::unique_ptr<Object> Clone() const { return std::make_unique<ProjectileComponent>(*this); }
	virtual ~ProjectileComponent();

	void Create() override;
	virtual void Update() override;

	virtual void OnCollisionEnter(const nc::Event& event);
	virtual void OnCollisionExit(const nc::Event& event);

	virtual bool Write(const rapidjson::Value& value) const override;
	virtual bool Read(const rapidjson::Value& value) override;
public:
	bool init = false;
	int speed = 100;
	nc::Vector2 fire2{ 0 };
	float lifetime{ 2 };
};
