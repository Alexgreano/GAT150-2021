#pragma once
#include "Component.h"
#include "Math/Vector2.h"
#include "Core/Serializable.h"

namespace nc {
	class PhysicsComponent : public Component {
	public:
		void Update() override;
		virtual void ApplyForce(const Vector2& force) { acceleration += force; };

		// Inherited via ISerializable
		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;
	public:
		Vector2 velocity;
		Vector2 acceleration;
		//Vector2 force;
		float damping = 1;
	};
}