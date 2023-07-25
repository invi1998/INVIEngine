#include "Input.h"

FCaptureOnMouseDelegate MouseDownDelegate;		// 鼠标按下
FCaptureOnMouseDelegate MouseUpDelegate;		// 鼠标抬起
FCaptureOnMouseDelegate MouseMoveDelegate;		// 鼠标移动


bool FInput::IsKeyPressed(KeyCode keycode)
{
	const bool state = GetAsyncKeyState(keycode) & 0x8000;
	return state;
}

bool FInput::IsMouseButtonPressed(int button)
{
	return true;
}

float FInput::GetMouseX()
{
	return 0.f;
}

float FInput::GetMouseY()
{
	return 0.f;
}

std::pair<float, float> FInput::GetMousePosition()
{
	return { 1.f, 1.f };
}
