#include "Actor.h"
#include "Graphics/Renderer.h"
#include "Component/GraphicsComponent .h"
#include <algorithm>

namespace nc {
	void nc::Actor::Update(float dt)
	{
		std::for_each(components.begin(), components.end(), [](auto& component) {component->Update(); });

		transform.Update();
		std::for_each(children.begin(), children.end(), [](auto& child) {child->transform.Update(child->parent->transform.matrix); });
	}

	void nc::Actor::Draw(Renderer* renderer)
	{
		//renderer->Draw(texture, transform);
		std::for_each(components.begin(), components.end(), [renderer](auto& component) {
			if (dynamic_cast<GraphicsComponent*>(component.get())) {
				dynamic_cast<GraphicsComponent*>(component.get())->Draw(renderer);
			}
			});
		
		std::for_each(children.begin(), children.end(), [renderer](auto& child) { child->Draw(renderer); });
	}

	void Actor::AddChild(std::unique_ptr<Actor> actor)
	{
		actor->parent = this;
		children.push_back(std::move(actor));
	}

	float Actor::GetRadius()
	{
		return 0;
	}


}
