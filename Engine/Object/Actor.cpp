#include "Actor.h"
#include "Graphics/Renderer.h"
#include <algorithm>

namespace nc {
	void nc::Actor::Update(float dt)
	{
		/*transform.rotation += 180.0f * dt;*/
		//transform.position.x += 100.0f * dt ;

		transform.Update();
		std::for_each(children.begin(), children.end(), [](auto& child) {child->transform.Update(child->parent->transform.matrix); });
	}

	void nc::Actor::Draw(Renderer* renderer)
	{
		//renderer->Draw(texture, transform);
		if (texture)
		{
			renderer->Draw(texture, transform);
		}
		else
		{
			//std::cout << "no texture" << std::endl;
		}
		std::for_each(children.begin(), children.end(), [renderer](auto& child) { child->Draw(renderer); });
	}

	void Actor::AddChild(std::unique_ptr<Actor> actor)
	{
		actor->parent = this;
		children.push_back(std::move(actor));
	}

	float Actor::GetRadius()
	{
		return (texture) ? texture->GetSize().Length() * 0.5f * transform.scale.x : 0;
	}


}
