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
	static bool IsKeyPressed(KeyCode keycode);
	static bool IsMouseButtonPressed(int button);

	static float GetMouseX();
	static float GetMouseY();

	static std::pair<float, float> GetMousePosition();
};

