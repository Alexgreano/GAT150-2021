#pragma once
#include "Framework/System.h"
#include <string>
#include <functional>
#include <map>
#include <list>
#include <variant>

namespace nc {
	struct Event {
		std::string name;
		std::variant<int, bool, float, std::string> data;
	};

	class EventSystem : public System {
	public:
		using funciton_t = std::function<void(const Event&)>;

		// Inherited via System
		virtual void Startup() override;
		virtual void Shutdown() override;
		virtual void Update(float dt) override;

		void Subscribe(const std::string& name, funciton_t funciton);
		void Notify(const Event& event);

	private:
		struct Observer {
			funciton_t function;
		};

	private:
		std::map<std::string, std::list<Observer>> observers;
	};

}