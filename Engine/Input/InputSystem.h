#pragma once
#include "Framework/System.h"
#include <vector>
#include <SDL_stdinc.h>
#include <Math/Vector2.h>
#include <array>

namespace nc{
	class InputSystem : public System {
	public:
		enum class eKeyState { Idle, Pressed, Held, Release };

		enum class eMouseButton {
			Left, Middle, Right
		};

		eKeyState GetKeyState(int id);
		bool IsKeyDown(int id) const { return keyboardState[id]; }
		bool IsPreviousKeyDown(int id) const { return prevKeyboardState[id]; }

		const Vector2& GetMousePosition() { return mousePosition; }
		bool isButtonDown(int id) { return mouseButtonState[id]; }
		bool isPrevButtonDown(int id) { return prevMouseButtonState[id]; }
		eKeyState GetButtonState(int id);

	private:
		std::vector<Uint8> keyboardState;
		std::vector<Uint8> prevKeyboardState;
		int numKeys;

		Vector2 mousePosition;
		//Uint8 MouseButtonState;
		std::array<Uint8, 3> mouseButtonState;
		std::array<Uint8, 3> prevMouseButtonState;
		//Uint8 PrevMouseButtonState;

		// Inherited via System
		virtual void Startup() override;
		virtual void Shutdown() override;
		virtual void Update(float dt) override;
	};

}