#pragma once
#include "Framework/System.h"
#include <vector>
#include <SDL_stdinc.h>

namespace nc{
	class InputSystem : public System {
	public:
		enum class eKeyState { Idle, Pressed, Held, Release };

		eKeyState GetKeyState(int id);
		bool IsKeyDown(int id);
		bool IsPreviousKeyDown(int id);


	private:
		std::vector<Uint8> keyboardState;
		std::vector<Uint8> prevKeyboardState;
		int numKeys;

		// Inherited via System
		virtual void Startup() override;
		virtual void Shutdown() override;
		virtual void Update(float dt) override;
	};

}