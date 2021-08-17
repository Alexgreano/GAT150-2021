#include "SpriteComponent.h"
#include "Graphics/Renderer.h"
#include "Object/Actor.h"

namespace nc {
	void SpriteCompnent::Update()
	{
	}
	void SpriteCompnent::Draw(Renderer* renderer)
	{
		renderer->Draw(texture, owner->transform);
	}
}