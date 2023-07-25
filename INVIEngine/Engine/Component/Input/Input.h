#pragma once

#include "EngineMinimal.h"

#include "KeyCode.h"

// �������ಥ����������ֵΪvoid, ����Ϊx,y��
DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnMouseDelegate, void, int, int);

extern FCaptureOnMouseDelegate MouseDownDelegate;		// ��갴��
extern FCaptureOnMouseDelegate MouseUpDelegate;			// ���̧��
extern FCaptureOnMouseDelegate MouseMoveDelegate;		// ����ƶ�

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

