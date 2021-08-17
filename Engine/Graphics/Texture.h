#pragma once
#include "Resource/Resource.h"
#include "Math/Vector2.h"
#include <SDL.h>
//#include <Graphics/Renderer.h>

namespace nc {
	class Renderer;
	class Texture : public Resource {
	public:
		bool Load(const std::string& name, void* data) override;

		Vector2 GetSize() const; 

		friend class Renderer;
		Texture() {}
		Texture(Renderer* renderer);

		bool Create(SDL_Surface* surface);

	private:
		SDL_Texture* texture{ nullptr };
		SDL_Renderer* renderer{ nullptr };
	};
}