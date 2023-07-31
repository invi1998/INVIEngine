#pragma once

#include "KeyCode.h"

class FInput
{
public:
	static bool IsKeyPressed(KeyCode keycode);
	static bool IsMouseButtonPressed(int button);

	static float GetMouseX();
	static float GetMouseY();

	static std::pair<float, float> GetMousePosition();
};

