#pragma once

#include "KeyCode.h"
#include "MouseCode.h"

DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnMousesWheelsDelegate, void, int, int, float);
DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnMousesMoveDelegate, void, int, int);
DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnRMousesButtonDownDelegate, void, int, int);	// ����Ҽ������¼�
DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnLMousesButtonDownDelegate, void, int, int);	// �����������¼�
DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnRMousesButtonUpDelegate, void, int, int);	// ����Ҽ�̧���¼�
DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnLMousesButtonUpDelegate, void, int, int);	// ������̧���¼�

extern FCaptureOnMousesWheelsDelegate MousesWheelsDelegate;
extern FCaptureOnMousesMoveDelegate MouseMoveDelegate;
extern FCaptureOnRMousesButtonDownDelegate RightMouseDownDelegate;	// �Ҽ�����¼�
extern FCaptureOnLMousesButtonDownDelegate LeftMouseDownDelegate;	// �������¼�
extern FCaptureOnRMousesButtonUpDelegate RightMouseUpDelegate;		// �Ҽ����̧���¼�
extern FCaptureOnLMousesButtonUpDelegate LeftMouseUpDelegate;		// ������̧���¼�

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


