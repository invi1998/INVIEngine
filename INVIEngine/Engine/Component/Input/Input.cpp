#include "Input.h"

FCaptureOnMouseDelegate MouseDownDelegate;		// ��갴��
FCaptureOnMouseDelegate MouseUpDelegate;		// ���̧��
FCaptureOnMouseDelegate MouseMoveDelegate;		// ����ƶ�


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
