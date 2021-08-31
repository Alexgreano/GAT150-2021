#pragma once
#include "Component/Component.h"
#include "Engine.h"

	class PickUpComponent: public nc::Component {
	public:
		std::unique_ptr<Object> Clone() const { return std::make_unique<PickUpComponent>(*this); }
		virtual ~PickUpComponent(); 

		void Create() override;
		virtual void Update() override;

		virtual void OnCollisionEnter(const nc::Event& event);
		virtual void OnCollisionExit(const nc::Event& event);

		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;
	
	};