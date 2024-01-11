#pragma once

#include "KeyCode.h"
#include "MouseCode.h"

DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnMousesWheelsDelegate, void, int, int, float);
DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnMousesMoveDelegate, void, int, int);

extern FCaptureOnMousesWheelsDelegate MousesWheelsDelegate;
extern FCaptureOnMousesMoveDelegate MouseMoveDelegate;

class FInput
{
public:
	static bool IsKeyPressed(uint16_t keycode);
	static bool IsMouseButtonPressed(int button);
	static bool IsKeyReleased(uint16_t keycode);

	static float GetMouseX();
	static float GetMouseY();

	static float getYOffset();

	static bool IsMouseButtonClicked(int button);

	static std::pair<float, float> GetMousePosition();

	// static void OnMouseMoved(int x, int y);
};


