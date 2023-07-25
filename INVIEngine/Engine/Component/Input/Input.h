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
	FInput() = default;

public:
	bool IsKeyPressed(KeyCode keycode);
	bool IsMouseButtonPressed(int button);

	float GetMouseX();
	float GetMouseY();

	std::pair<float, float> GetMousePosition();
};

