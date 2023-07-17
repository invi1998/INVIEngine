#pragma once

#include "EngineMinimal.h"

#include "KeyCode.h"

// 定义鼠标多播函数（返回值为void, 参数为x,y）
DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnMouseDelegate, void, int, int);

extern FCaptureOnMouseDelegate MouseDownDelegate;		// 鼠标按下
extern FCaptureOnMouseDelegate MouseUpDelegate;			// 鼠标抬起
extern FCaptureOnMouseDelegate MouseMoveDelegate;		// 鼠标移动

class FInput
{
public:
	static bool IsKeyPressed(KeyCode keycode);
	static bool IsMouseButtonPressed(int button);

	static float GetMouseX();
	static float GetMouseY();

	static std::pair<float, float> GetMousePosition();
};

