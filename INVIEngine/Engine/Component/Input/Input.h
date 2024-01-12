#pragma once

#include "KeyCode.h"
#include "MouseCode.h"

DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnMousesWheelsDelegate, void, int, int, float);
DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnMousesMoveDelegate, void, int, int);
DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnRMousesButtonDownDelegate, void, int, int);	// 鼠标右键按下事件
DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnLMousesButtonDownDelegate, void, int, int);	// 鼠标左键按下事件
DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnRMousesButtonUpDelegate, void, int, int);	// 鼠标右键抬起事件
DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnLMousesButtonUpDelegate, void, int, int);	// 鼠标左键抬起事件

extern FCaptureOnMousesWheelsDelegate MousesWheelsDelegate;
extern FCaptureOnMousesMoveDelegate MouseMoveDelegate;
extern FCaptureOnRMousesButtonDownDelegate RightMouseDownDelegate;	// 右键鼠标事件
extern FCaptureOnLMousesButtonDownDelegate LeftMouseDownDelegate;	// 左键鼠标事件
extern FCaptureOnRMousesButtonUpDelegate RightMouseUpDelegate;		// 右键鼠标抬起事件
extern FCaptureOnLMousesButtonUpDelegate LeftMouseUpDelegate;		// 左键鼠标抬起事件

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

};


