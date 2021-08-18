#pragma once
//#include "Graphics/ParticleSystem.h"
#include "Audio/AudioSystem.h"

//input
#include "Input/InputSystem.h"

//core
#include "Core/Utilities.h"
#include "Core/FileSystem.h"
#include "Core/Timer.h"

//math
#include "Math/Vector2.h"
#include "Math/Color.h"
#include "Math/Random.h"
#include "Math/MathUtilis.h"
#include "Math/Transform.h"

//graphics
#include "Graphics/Renderer.h"
#include "Graphics/Texture.h"
#include "Graphics/ParticleSystem.h"


//objects
#include "Object/Actor.h"
#include "Object/Scene.h"

#include <vector>
#include <memory>
#include <algorithm>

//component
#include "Component/SpriteComponent.h"
#include "Component/PhysicsComponent.h"
#include "Component/SpriteAnimationComponent.h"

#define NOMINMAX
//#include "core.h"

//framework
#include "Framework/EventSystem.h"
#include "Resource/ResourceSystem.h"

namespace nc {
	class Engine {
	public:
		void Startup();
		void Shutdown();

		void Update();
		void Draw(Renderer* renderer);

		template<typename T>
		T* Get();

	public:
		FrameTimer time;

	private:
		std::vector<std::unique_ptr<System>> systems;
	};
	template<typename T>
	inline T* Engine::Get()
	{
		for (auto& system : systems) {
			if (dynamic_cast<T*>(system.get())) return dynamic_cast<T*>(system.get());
		}
		return nullptr;
	}
}