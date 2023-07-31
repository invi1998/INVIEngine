#pragma once

#include "KeyCode.h"
#include "MouseCode.h"

class FInput
{
public:
	static bool IsKeyPressed(uint16_t keycode);
	static bool IsMouseButtonPressed(int button);

	static float GetMouseX();
	static float GetMouseY();

	static std::pair<float, float> GetMousePosition();
};

