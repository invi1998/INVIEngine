#include "EngineMinimal.h"
#include "Component/Input/Input.h"

bool FInput::IsKeyPressed(uint16_t keycode)
{
	const bool state = GetAsyncKeyState(keycode) & 0x8000;
	return state;
}

bool FInput::IsMouseButtonPressed(int button)
{
	const bool state = GetAsyncKeyState(button) & 0x8000;
	return state;
}

float FInput::GetMouseX()
{
	auto [x, y] = GetMousePosition();
	return x;
}

float FInput::GetMouseY()
{
	auto [x, y] = GetMousePosition();
	return y;
}

std::pair<float, float> FInput::GetMousePosition()
{
	POINT pt;
	GetCursorPos(&pt);
	return { static_cast<float>(pt.x), static_cast<float>(pt.y) };
}
